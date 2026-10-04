#include <glm/glm.hpp>
#include "../VAO.hpp"
#include "../../shaders/ProgramShaders.hpp"
#include <jpl/logger/Logger.hpp>

namespace jpl::_graphics::_engine::ir {
    template<typename K, typename H, unsigned int SIZE>
    Instancer<K, H, SIZE>::Instancer(jpl::_graphics::_shaders::ProgramShaders *ps, jpl::_graphics::_engine::VAO *vao, const std::array<K, SIZE>& normals, unsigned int capacity) {
        this->vao = vao;
        this->ps = ps;
        this->vao->bind();
        this->capacity = capacity;
        this->count = 0;
        glGenBuffers(1, &this->allInstanceSSBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->allInstanceSSBO);
        glBufferStorage(GL_SHADER_STORAGE_BUFFER, this->capacity*sizeof(InstanceData), nullptr, GL_DYNAMIC_STORAGE_BIT);
        glGenBuffers(1, &this->visibleIndicesSSBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->visibleIndicesSSBO);
        glBufferStorage(GL_SHADER_STORAGE_BUFFER, this->capacity*sizeof(unsigned int), nullptr, GL_DYNAMIC_STORAGE_BIT);
        glGenBuffers(1, &this->atomicCounterBuffer);
        glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, this->atomicCounterBuffer);
        glBufferStorage(GL_ATOMIC_COUNTER_BUFFER, sizeof(unsigned int), nullptr, GL_DYNAMIC_STORAGE_BIT);
        glGenBuffers(1, &this->drawCommandBuffer);
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, this->drawCommandBuffer);
        DrawElementsIndirectCommand cmd {_shapes::Cube::SIZE_INDICES, 0, 0, 0,0 };
        glBufferStorage(GL_DRAW_INDIRECT_BUFFER, sizeof(DrawElementsIndirectCommand), &cmd, GL_DYNAMIC_STORAGE_BIT);
        this->vec.resize(this->capacity);
        this->cpuMirror.resize(this->capacity);
        this->normals = normals;
        this->texturesRegistered = 0;
    }

    template<typename K, typename H, unsigned int SIZE>
    void Instancer<K, H, SIZE>::addID(unsigned int objectId, const _camera::CameraFrustum &cf, const InstanceData &id, const K &k) {
        if (this->count >= this->capacity)
            this->growBuffers();
        size_t slot = this->count++;
        this->map[k] = slot;
        this->vec[slot] = k;
        this->cpuMirror[slot] = id;
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->allInstanceSSBO);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, slot*sizeof(InstanceData), sizeof(InstanceData), &id);
        this->occlusionCulling(k, objectId);
    }

    template<typename K, typename H, unsigned int SIZE>
    void Instancer<K, H, SIZE>::removeID(unsigned int objectId, const _camera::CameraFrustum &cf, const K &k) {
        size_t slot = this->map.at(k);
        size_t lastSlot = this->count-1;
        if (slot != lastSlot) {
            K lastPos = this->vec[lastSlot];
            InstanceData movedData = this->cpuMirror[lastSlot];
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->allInstanceSSBO);
            glBufferSubData(GL_SHADER_STORAGE_BUFFER, slot*sizeof(InstanceData), sizeof(InstanceData), &movedData);
            this->map.insert_or_assign(lastPos, slot);
            this->vec[slot] = lastPos;
            this->cpuMirror[slot] = movedData;
        }
        this->map.erase(k);
        --this->count;
        //occlusion culling on neighbours
        for (unsigned int i = 0; i < SIZE; i++) {
            K neighbour = k + this->normals[i];
            if (this->map.contains(neighbour))
                this->occlusionCulling(neighbour, objectId);
        }
    }

    template<typename K, typename H, unsigned int SIZE>
    void Instancer<K, H, SIZE>::occlusionCulling(const K &k, unsigned int objectId) {
        if (this->map.contains(k)) {
            size_t slot = this->map.at(k);
            InstanceData &id = this->cpuMirror[slot];
            const auto& original = this->originalTextures.at(objectId);
            for (unsigned int i = 0; i < SIZE; i++) {
                K neighbour = k + this->normals[i];
                if (this->map.contains(neighbour)) {
                    id.faceIdTexture[i] = -1;
                }else {
                    id.faceIdTexture[i] = original[i];
                }
            }
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->allInstanceSSBO);
            glBufferSubData(GL_SHADER_STORAGE_BUFFER, slot*sizeof(InstanceData), sizeof(InstanceData), &id);
        }
    }

    template<typename K, typename H, unsigned int SIZE>
    void Instancer<K, H, SIZE>::growBuffers() {
        size_t newCap = this->capacity*2;
        unsigned int newSSBO;
        glGenBuffers(1, &newSSBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, newSSBO);
        glBufferStorage(GL_SHADER_STORAGE_BUFFER, newCap*sizeof(InstanceData), nullptr, GL_DYNAMIC_STORAGE_BIT);
        //let's copy all previous instanced
        glBindBuffer(GL_COPY_READ_BUFFER, this->allInstanceSSBO);
        glBindBuffer(GL_COPY_WRITE_BUFFER, newSSBO);
        glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, this->count*sizeof(InstanceData));
        //delete old buffer
        glDeleteBuffers(1, &this->allInstanceSSBO);
        this->allInstanceSSBO = newSSBO;
        //Now let's grow visibleSSBO
        unsigned int newVisibleSSBO;
        glGenBuffers(1, &newVisibleSSBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, newVisibleSSBO);
        //No glBufferSubData 'cause the buffer will be filled on the next compute shader dispatch
        glBufferStorage(GL_SHADER_STORAGE_BUFFER, newCap*sizeof(InstanceData), nullptr, GL_DYNAMIC_STORAGE_BIT);
        glDeleteBuffers(1, &this->visibleIndicesSSBO);
        this->visibleIndicesSSBO = newVisibleSSBO;
        this->capacity = newCap;
        this->cpuMirror.resize(newCap);
        this->vec.resize(newCap);
    }

    template<typename K, typename H, unsigned int SIZE>
    void Instancer<K, H, SIZE>::initializeTextureArray(unsigned int count, unsigned int w, unsigned int h) {
        glGenTextures(1, &this->textureArray);
        glBindTexture(GL_TEXTURE_2D_ARRAY, this->textureArray);
        glTexImage3D(
            GL_TEXTURE_2D_ARRAY,
            0,
            GL_RGBA8,
            w,
            h,
            count,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            nullptr
        );
        this->maxRegisterTextures = count;
        this->widthTexture = w;
        this->heightTexture = h;
    }

    template<typename K, typename H, unsigned int SIZE>
    void Instancer<K, H, SIZE>::addTextures(size_t id, std::vector<_texture::Texture *> *textures, const std::array<unsigned int, SIZE>& faces) {
        if (textures->empty())
            throw jpl::_exception::IllegalArgumentException("Cannot pass empty texture vector");
        if (this->texturesRegistered+textures->size() > this->maxRegisterTextures)
            throw jpl::_exception::IllegalArgumentException("Adding given textures vector of " + std::to_string(textures->size()) + ", glTexturesArray will exced its capacity " + std::to_string(this->maxRegisterTextures));
        for (size_t i = 0; i < textures->size(); i++) {
            glTexSubImage3D(
                GL_TEXTURE_2D_ARRAY, 0, 0, 0, this->texturesRegistered+i, this->widthTexture, this->heightTexture, 1,
                GL_RGBA, GL_UNSIGNED_BYTE, textures->at(i)->getData()
            );
        }
        for (size_t i = 0; i < SIZE; i++) {
            size_t crFace = faces[i];
            if (crFace >= textures->size())
                throw jpl::_exception::IllegalArgumentException("faces array contains at " + std::to_string(i) + " value " + std::to_string(crFace) + " which is greater than textures vector size " + std::to_string(textures->size()));
            this->originalTextures[id][i] = faces.at(i)+this->texturesRegistered;
        }
        this->texturesRegistered += textures->size();
    }

    template<typename K, typename H, unsigned int SIZE>
    void Instancer<K, H, SIZE>::terminateAddingTextures() noexcept {
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    }
}