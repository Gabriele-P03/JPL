/**
 * ProgramShaders performs a linkage mechanism between a program and a set of shaders.
 * It also allows to manage the shaders set
 */

#ifndef PROGRAMSHADERS_GRAPHICS_JPL
#define PROGRAMSHADERS_GRAPHICS_JPL

#include <jpl/utils/structure/list/LinkedList.hpp>
#include <GL/glew.h>
#include "../utils/Error.hpp"

namespace jpl{
    namespace _graphics{
        namespace _shaders{

            class Shader;

            /**
             * This non-member function has been declared (and defined in Shader.cpp) in order to
             * garantuee maximum compatibility in case of recursive included header files
             * @throw IllegalArgumentException if shader is nullptr
             * @throw IllegalStateException if shader has not been created, yet, calling probeShader()
             * @return shader's index
             */
            extern unsigned int getShaderIndexByShader(Shader* shader);

            class ProgramShaders{

                protected:

                    const unsigned int program;
                    jpl::_utils::_collections::_list::LinkedList<Shader*>* shaders;

                public:

                    /**
                     * Instance a new ProgramShader object and call glCreateProgram by itselves
                     */
                    ProgramShaders();

                    /**
                     * @param program
                     */
                    ProgramShaders(unsigned int program);

                    ~ProgramShaders();
                    
                    /**
                     * Add new shader to the current list
                     * @param shader
                     * @throw IllegalArgumentException if shader is nullptr
                     * @throw IllegalArgumentException if shader has been already inserted into this list
                     * @throw RuntimeException for any error occurred during attaching shader 
                     */
                    void addNewShader(Shader* shader);

                    /**
                     * Detach the given shader
                     * @param shader
                     * @throw IllegalArgumentException if shader is nullptr
                     * @throw RuntimeException is shader was not found in list
                     */
                    void detachShader(Shader* shader);
                    /**
                     * Detach shader with the given shaderIndex
                     * @param shaderIndex
                     * @IllegalArgumentException if no shader has been found with the given shaderIndex
                     * @throw RuntimeException for any errors occurred
                     */
                    void detachShader(unsigned int shaderIndex);

                    size_t getShadersSize() const noexcept{
                        return this->shaders->getSize();
                    }

                    Shader* getShaderByIndex(size_t i) const noexcept{
                        return this->shaders->get(i);
                    }
                
                    unsigned int getProgramIndex() const noexcept {
                        return this->program;
                    }
            };
        }
    }
}


#endif