/**
 * ProgramShaders performs a linkage mechanism between a program and a set of shaders.
 * It also allows to manage the shaders set
 */

#ifndef PROGRAMSHADERS_GRAPHICS_JPL
#define PROGRAMSHADERS_GRAPHICS_JPL

#include <unordered_map>
#include <GL/glew.h>
#include <jpl/exception/runtime/IllegalArgumentException.hpp>
#include <jpl/logger/Logger.hpp>
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
                    std::unordered_map<std::string, Shader* const> shaders;

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
                     * Add new shader to the map
                     * @param identifier
                     * @param shader
                     * @throw IllegalArgumentException if shader is nullptr
                     * @throw IllegalArgumentException if shader has been already inserted into this list
                     * @throw RuntimeException for any error occurred during attaching shader 
                     */
                    void addNewShader(const std::string &identifier, Shader* shader);

                    Shader* getShaderByIdentifier(const std::string &identifier) const;

                    /**
                     * Detach the given shader
                     * @param identifier
                     * @throw RuntimeException is shader was not found
                     */
                    void detachShader(const std::string &identifier);

                    virtual void link() const;
                    virtual void use() const;
                
                    unsigned int getProgramIndex() const noexcept {
                        return this->program;
                    }
            };
        }
    }
}


#endif