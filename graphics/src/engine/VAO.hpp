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
                const unsigned int vbo;
                VBO(const unsigned int vbo) : vbo(vbo){}
                ~VBO() = default;
                void bind() const noexcept{
                    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
                }
            };

            struct EBO{
                const unsigned int ebo;
                EBO(const unsigned int ebo) : ebo(ebo){}
                ~EBO() = default;
                void bind() const noexcept{
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
                }
            };

            struct VAO{
                const unsigned int vao;
                std::vector<VBO*> VBOs;
                std::vector<EBO*> EBOs;
                VAO(const unsigned int vao);
                ~VAO();

                VBO* const addVBO();
                VBO* const addVBO(const unsigned int VBO);
                VBO* const addVBO(VBO* const vbo);

                EBO* const addEBO();
                EBO* const addEBO(const unsigned int EBO);
                EBO* const addEBO(EBO* const vbo);

                void bind() const noexcept{
                    glBindVertexArray(this->vao);
                }
            };
            

        }

    }
}

#endif