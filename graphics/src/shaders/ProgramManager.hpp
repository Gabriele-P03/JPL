/**
 * In the class declared below, a mechanism to manage set of (shader) program - with theirs shaders -
 * will be implemented
 */

#ifndef SHADERPROGRAM_GRAPHICS_JPL
#define SHADERPROGRAM_GRAPHICS_JPL

#include <unordered_map>
#include "ProgramShaders.hpp"
#include <jpl/exception/runtime/NotFoundException.hpp>
#include <tuple>


namespace jpl{
    namespace _graphics{
        namespace _shaders{

            class ProgramManager{
                
                protected:
                    std::unordered_map<std::string, ProgramShaders* const> programShaders;

                public:

                    ProgramManager();
                    ~ProgramManager();

                    /**
                     * @param program
                     * @throw IllegalArgumentException if program is 0 (program not valid)
                     * @return ProgramShaders with @program or nullptr if none exists
                     */
                    ProgramShaders* const getProgramShadersByProgram(unsigned int program) const;

                    /**
                     * @param identifier
                     * @return ProgramShaders 
                     * @throw NotFoundException if none program shaders is found
                     */
                    ProgramShaders* const getProgramShadersByIdentifier(const std::string &identifier) const;

                    /**
                     * @param identifier
                     * @param programShaders
                     * @throw IllegalArgumentException if programShaders is nullptr
                     * @throw IllegalArgumentException if programShaders with the same identifier has been already inserted
                     * @throw IllegalArgumentException if programShaders pointer has been already inserted
                     */
                    void addProgramShaders(const std::string &identifier, ProgramShaders* const programShaders);

                    /**
                     * @param identifier
                     * @throw NotFoundException if no ProgramShaders with the given name has been found
                     */
                    void removeProgramShaders(const std::string &identifier);

                    static ProgramManager* INSTANCE;
                    static void initializeProgramManager();

            };
        }
    }
}

#endif