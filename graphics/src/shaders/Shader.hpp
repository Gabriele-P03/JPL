/**
 * THis is the abstraction of a shader program
 */
#ifndef SHADER_GRAPHICS_JPL
#define SHADER_GRAPHICS_JPL

#include <jpl/exception/runtime/IllegalStateException.hpp>

#include <GL/glew.h>
#include <GL/gl.h>

#include <jpl/utils/FilesUtils.hpp>
#include "../utils/Error.hpp"

namespace jpl{
    namespace _graphics{
        namespace _shaders{

            /**
             * Load the shader program from the given stream
             * @param s file stream
             * @return const char* containing raw program
             */
            extern const char* loadProgram(std::fstream* s);

            class Shader{

                protected:
                    unsigned int shaderIndex;

                    const char* rawProgram;
                    int sizeRawProgram;

                    unsigned int shaderType;

                    //True if source code has been attached to shader program and compiled successfully
                    bool compiled;

                public:

                    /**
                     * Load a shader program from file
                     * @param s file stream
                     */
                    Shader(std::fstream* s);

                    /**
                     * Load a shader program from buffer
                     * @param rawProgram buffer
                     * @param sizeRawProgram buffer's size
                     */
                    Shader(const char* rawProgram, int sizeRawProgram);

                    ~Shader();

                    /**
                     * This should be the first method to be called after have instanced a Shader object.
                     * It does create shader program, attach source code and compile it
                     * @param shaderType shader type
                     */
                    virtual void probeShader(int shaderType);


                    unsigned int getShaderIndexProgram() const noexcept{
                        return this->shaderIndex;
                    }

                    /**
                     * @throw IllegalStateException if probeShader() has not been called before or any error occurred during its performing
                     * @return type of current shader program 
                     */
                    unsigned int getShaderType() const;

                    bool isCompiled() const noexcept{
                        return this->compiled;
                    }
            };

            /**
             * This non-member function has been declared (and defined in Shader.cpp) in order to
             * garantuee maximum compatibility in case of recursive included header files
             * @throw IllegalArgumentException if shader is nullptr
             * @throw IllegalStateException if shader has not been created, yet, calling probeShader()
             * @return shader's index
             */
            extern unsigned int getShaderIndexByShader(Shader* shader);
        }
    }
}

#endif