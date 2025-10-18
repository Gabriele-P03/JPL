/**
 * In the class declared below, a mechanism to manage set of (shader) program - with theirs shaders -
 * will be implemented
 */

#ifndef SHADERPROGRAM_GRAPHICS_JPL
#define SHADERPROGRAM_GRAPHICS_JPL

#include <jpl/utils/structure/list/LinkedList.hpp>
#include "ProgramShaders.hpp"


namespace jpl{
    namespace _graphics{
        namespace _shaders{

            class ProgramManager{
                
                protected:
                    jpl::_utils::_collections::_list::LinkedList<ProgramShaders*>* programShaders;

                public:

                    ProgramManager();
                    ~ProgramManager();

                    /**
                     * @param program
                     * @throw IllegalArgumentException if program is 0 (program not valid)
                     * @return ProgramShaders with @program or nullptr if none exists
                     */
                    ProgramShaders* getProgramShadersByProgram(unsigned int program);

                    /**
                     * @param programShaders
                     * @throw IllegalArgumentException if programShaders is nullptr
                     * @throw IllegalArgumentException if a programShaders with the same program index has been already inserted 
                     * @throw IllegalArgumentException if programShaders pointer has been already inserted
                     */
                    void addProgramShaders(ProgramShaders* programShaders);
                    /**
                     * If the managed list already contains a ProgramShaders object with the same @program, @shader will be inserted into it
                     * @param program
                     * @param shader which will be always attached
                     * @throw IllegalArgumentException if program is equal to 0 (program not valid)
                     * @throw IllegalArgumentException if shader is nullptr
                     * @throw IllegalArgumentException if a shader with the same pointer has been already inserted
                     * @throw RuntimeException if shader coult not be attached to new ProgramShader
                     */
                    void addProgramShader(unsigned int program, Shader* shader);
            };
        }
    }
}

#endif