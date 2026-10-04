/**
 * Instance Rendering is a technique which can be used to render thousands times the same object (i.e. the same VBO).
 *
 * You have to declare your own InstanceRendering template giving the following template args:
 * K: your position class (i.e. that class which represents position of your objects)
 * H: K's hashing to use for map
 * SIZE: amount of faces of your object
 *
 * When passing textures via initTexturesArray and addTextures remember that you may pass the same texture for every face as well
 *
 * Since this is just a framework, it is not able to foresee your uniforms' location; so, you have to declare them as first ones.
 * location 0 to 3 for modelView
 * location 4 for textures' id for top, bottom, left and right side
 * location 5 ""                 " front and back side
 */

#ifndef INSTANCE_RENDERING_GRAPHICS_JPL
#define INSTANCE_RENDERING_GRAPHICS_JPL

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../VAO.hpp"
#include "../../shaders/ProgramShaders.hpp"
#include <jpl/logger/Logger.hpp>
#include "../../texture/Texture.hpp"
#include "../../shapes/Cube.hpp"
#include "../camera/FrustumCulling.hpp"

namespace jpl::_graphics::_engine::ir{

    struct DrawElementsIndirectCommand {
        GLuint count;         // amount of indices to render (Cube::SIZE_INDICES)
        GLuint instanceCount; // instance amount. Written by GPU's compute shader
        GLuint firstIndex;    // index buffer offset
        GLint  baseVertex;
        GLuint baseInstance;
    };

    template<typename K, typename H, unsigned int SIZE>
    class Instancer{

        public:

            struct alignas(16) InstanceData{
                glm::mat4 modelMatrix;
                std::array<unsigned int, SIZE> faceIdTexture;
            };
            static_assert(sizeof(InstanceData)%16 == 0, "sizeof(InstanceData) must be aligned at 16 bytes. This assert should never be failed... Bug?!");

        protected:

            std::array<K, SIZE> normals;

            unsigned int allInstanceSSBO;
            unsigned int visibleIndicesSSBO;
            unsigned int atomicCounterBuffer;
            unsigned int drawCommandBuffer;

            size_t capacity;    //allocated slots in the buffer
            size_t count;       //slots actually used

            size_t texturesRegistered, maxRegisterTextures;
            unsigned int widthTexture, heightTexture;

            std::unordered_map<K, size_t, H> map;
            std::vector<K> vec;

            /**
            *   In case of addID and removeID, to retrieve data from GPU via glGetBufferData would cause a synchronous stall CPU-GPU.
            *   In order to prevent this, a mirror of the data passed to the GPU is rest here.
            *   It is already known that it could be a massive memory usage - regardless sizeof(InstanceData) is few bytes - but you know,
            *   in rendering, memory is never a massive usage rather than GPU's state changes
            */
            std::vector<InstanceData> cpuMirror;

            /*
             * Original buffer which stores all textures
             * InstanceData's faceIdTextures cannot be used as original one sice its values can be set to -1 for occlusion culling
             */
            std::unordered_map<size_t, std::array<unsigned int, SIZE>> originalTextures;

            unsigned int textureArray;
            VAO* vao;
            _shaders::ProgramShaders* ps;

        public:

            Instancer(jpl::_graphics::_shaders::ProgramShaders* ps, VAO* vao, const std::array<K, SIZE>& normals, unsigned int capacity);

            /**
             * Called by addID to extends SSBO by a factor of 2 (duplicate its sizeSS).
             */
            virtual void growBuffers();

            virtual void occlusionCulling(const K& k, unsigned int objectId);

            /**
             * Initialize texture array via glTexImage3D
             * @param count how many different textures it has to contain (you have to count even any different faces)
             * @param w textures' width
             * @param h textures' height
            */
            virtual void initializeTextureArray(unsigned int count, unsigned int w, unsigned int h);

            /**
             * @param id of the object
             * @oaram textures vector of all textures of the given object
             * @param faces array of textures' indices (e.g. {0,0,0,0,0,0} means that all the six faces have the same textures - which is passed into textures )
            */
            virtual void addTextures(size_t id, std::vector<_texture::Texture*>* textures, const std::array<unsigned int, SIZE> &faces);

            /**
             *  To be called once all textures have been sent via addTextures().
             *  It unbinds texture array buffer and calls glTexParameteri and glGenerateMipmap
            */
            virtual void terminateAddingTextures() noexcept;

            virtual void addID(unsigned int objectId, const _camera::CameraFrustum &cf, const InstanceData &id, const K& k);
            virtual void removeID(unsigned int objectId, const _camera::CameraFrustum &cf, const K& k);

            virtual void render(const glm::mat4 &view, const glm::mat4 &projection){
                this->ps->use();
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D_ARRAY, this->textureArray);
                glUniform1i(glGetUniformLocation(this->ps->getProgramIndex(), "textureArray"), 0);
                this->vao->bind();
                glUniformMatrix4fv(glGetUniformLocation(this->ps->getProgramIndex(), "view"), 1, GL_FALSE, glm::value_ptr(view));
                glUniformMatrix4fv(glGetUniformLocation(this->ps->getProgramIndex(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

                glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->allInstanceSSBO);
                glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, this->visibleIndicesSSBO);
                glBindBuffer(GL_DRAW_INDIRECT_BUFFER, this->drawCommandBuffer);
                GLuint instanceCountValue = static_cast<GLuint>(this->count);
                glBufferSubData(GL_DRAW_INDIRECT_BUFFER, offsetof(DrawElementsIndirectCommand, instanceCount), sizeof(GLuint), &instanceCountValue);

                glDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr);
            }

            virtual ~Instancer() = default;
    };
}

#include "InstanceRendering.tpp"

#endif