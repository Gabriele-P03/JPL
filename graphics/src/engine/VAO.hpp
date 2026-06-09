/**
 * Simple struct to link VAO with its VBOs
 */

#ifndef VAOSVBOS_GRAPHICS_JPL
#define VAOSVBOS_GRAPHICS_JPL

#include <jpl/exception/runtime/IllegalArgumentException.hpp>
#include <GL/glew.h>
#include <string.h>
#include <string>
#include <vector>

namespace jpl{
    namespace _graphics{
        namespace _engine{

            struct VBO{
                const std::string name;
                const unsigned int vbo;
                VBO(const std::string &name, const unsigned int vbo)
                    : name(name), vbo(vbo){
                }
                ~VBO() = default;
                void bind() const noexcept{
                    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
                }
            };

            struct EBO{
                const std::string name;
                const unsigned int ebo;
                EBO(const std::string &name, const unsigned int ebo)
                    : name(name), ebo(ebo){

                }
                ~EBO() = default;
                void bind() const noexcept{
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
                }
            };

            struct VAO{
                const std::string name;
                const unsigned int vao;
                std::vector<VBO*> VBOs;
                std::vector<EBO*> EBOs;
                VAO(const std::string &name, const unsigned int vao)
                    : name(name), vao(vao){

                }
                ~VAO() = default;

                VBO* addVBO(const std::string &name);
                VBO* addVBO(const std::string &name, const unsigned int VBO);
                void addVBO(VBO* vbo);
                VBO* getVBOByName(const std::string& name) noexcept;

                EBO* addEBO(const std::string &name);
                EBO* addEBO(const std::string &name, const unsigned int EBO);
                void addEBO(EBO* vbo);
                EBO* getEBOByName(const std::string& name) noexcept;

                void bind() const noexcept{
                    glBindVertexArray(this->vao);
                }
            };
            

        }

    }
}

#endif