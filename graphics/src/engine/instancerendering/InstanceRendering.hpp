/**
 * Instance Rendering is a technique which can be used to render thousands times the same object (i.e. the same VBO).
 *
 * Since this is just a framework, it is not able to foresee your uniforms' location; so, you have to declare them as first ones.
 * location 0 to 3 for modelView
 * location 4 for textures' id for top, bottom, left and right side
 * location 5 ""                 " front and back side
 */
#ifndef INSTANCE_RENDERING_GRAPHICS_JPL
#define INSTANCE_RENDERING_GRAPHICS_JPL

#include <glm/glm.hpp>
#include "../VAO.hpp"
#include "../../shaders/ProgramShaders.hpp"
#include <jpl/logger/Logger.hpp>
#include "../../texture/Texture.hpp"
#include "../../shapes/Cube.hpp"
#include "../camera/FrustumCulling.hpp"

namespace jpl::_graphics::_engine::ir{

    #pragma pack(push, 1)
    inline struct InstanceData{
        glm::mat4 modelMatrix;
        unsigned int faceIdTexture[6];
        size_t i;
    } ID;
    #pragma pack(pop)

    template<typename K, typename H>
    class Instancer{

        protected:

            std::unordered_map<K, InstanceData, H> map;
            std::vector<InstanceData> vec;
            VAO* vao;
            VBO* vbo;
            unsigned int textureArray;
            jpl::_graphics::_shaders::ProgramShaders* ps;

        public:
            Instancer(VAO* vao, jpl::_graphics::_shaders::ProgramShaders* ps) {
                this->vao = vao;
                this->ps = ps;
                this->vao->bind();
                this->vbo = vao->addVBO();
                this->vbo->bind();
                std::size_t stride = sizeof(InstanceData);
                std::size_t tex1Offset = sizeof(glm::mat4);
                std::size_t tex2Offset = tex1Offset + (4 * sizeof(unsigned int));

                std::size_t vec4Size = sizeof(glm::vec4);

                for (unsigned int i = 0; i < 4; i++) {
                    glEnableVertexAttribArray(i);
                    glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, stride, (void*)(i * vec4Size));
                    glVertexAttribDivisor(i, 1);
                }

                glEnableVertexAttribArray(4);
                glVertexAttribIPointer(4, 4, GL_UNSIGNED_INT, stride, (void*)tex1Offset);
                glVertexAttribDivisor(4, 1);

                glEnableVertexAttribArray(5);
                glVertexAttribIPointer(5, 2, GL_UNSIGNED_INT, stride, (void*)tex2Offset);
                glVertexAttribDivisor(5, 1);

                glBindVertexArray(0);
                glGenTextures(1, &this->textureArray);
            }

            virtual void addTextures(unsigned int w, unsigned int h, std::vector<_texture::Texture*>* textures) {
                size_t n = textures->size();
                glBindTexture(GL_TEXTURE_2D_ARRAY, this->textureArray);
                glTexImage3D(
                    GL_TEXTURE_2D_ARRAY,
                    0,
                    GL_RGBA8,
                    w,
                    h,
                    n,
                    0,
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    nullptr
                );
                for (size_t i = 0; i < n; i++) {
                    glTexSubImage3D(
                        GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, w, h, 1,
                        GL_RGBA, GL_UNSIGNED_BYTE, textures->at(i)->getData()
                    );
                }
                glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
                glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
                glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
            }

            virtual void addID(const _camera::CameraFrustum &cf, const InstanceData &id, const K& k, bool updateVBOFlag = true) {
                this->map.insert(std::pair<K, InstanceData>(std::move(k), std::move(id)));
                if (updateVBOFlag)
                    this->updateVBO(cf);
            }

            virtual void removeID(const _camera::CameraFrustum &cf, const K& k, bool updateVBOFlag = true) {
                this->map.erase(k);
                if (updateVBOFlag)
                    this->updateVBO(cf);
            }

            virtual void render() {
                this->ps->use();
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D_ARRAY, this->textureArray);
                glUniform1i(glGetUniformLocation(this->ps->getProgramIndex(), "textureArray"), 0);
                this->vao->bind();
                glDrawElementsInstanced(GL_TRIANGLES, jpl::_graphics::_shapes::Cube::SIZE_INDICES, GL_UNSIGNED_INT, nullptr, this->vec.size());
            }

            /**
             * Called by addID and removeID both to update VBO's vertices
             */
            virtual void updateVBO(const _camera::CameraFrustum &cf) {
                    this->vao->bind();
                    this->vbo->bind();
                    this->vec.clear();
                    for (auto& [k, id] : this->map) {
                        glm::vec3 blockPos = glm::vec3(id.modelMatrix[3]);
                        if (cf.isPointVisible(blockPos, 0.86f)) {
                            // block is within camera
                            this->vec.push_back(id);
                        }
                    }
                    glBufferData(GL_ARRAY_BUFFER, this->vec.size()*sizeof(InstanceData), this->vec.data(), GL_DYNAMIC_DRAW);
                }

            /**
             * Clear map and vec
             */
            virtual void clear() {
                this->map.clear();
                this->vec.clear();
            }
    };
}

#endif