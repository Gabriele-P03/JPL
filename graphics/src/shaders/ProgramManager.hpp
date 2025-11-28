/**
 * In the class declared below, a mechanism to manage set of (shader) program - with theirs shaders -
 * will be implemented
 */

#ifndef SHADERPROGRAM_GRAPHICS_JPL
#define SHADERPROGRAM_GRAPHICS_JPL

#include <jpl/utils/structure/list/LinkedList.hpp>
#include "ProgramShaders.hpp"
#include <tuple>


namespace jpl{
    namespace _graphics{
        namespace _shaders{

            class ProgramManager{
                
                protected:
                    jpl::_utils::_collections::_list::LinkedList<
                        std::tuple<
                            std::string,    //identifier
                            ProgramShaders*
                        >
                    >* programShaders;

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
                     * @param identifier
                     * @return ProgramShaders 
                     * @throw NotFoundException if none program shaders is found
                     */
                    ProgramShaders* getProgramShadersByIdentifier(const std::string &identifier);

                    /**
                     * @param identifier
                     * @param programShaders
                     * @throw IllegalArgumentException if programShaders is nullptr
                     * @throw IllegalArgumentException if a programShaders with the same program index has been already inserted 
                     * @throw IllegalArgumentException if programShaders pointer has been already inserted
                     */
                    void addProgramShaders(const std::string &identifier, ProgramShaders* programShaders);

            };
        }
    }
}

#endif