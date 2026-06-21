/**
 * VAOsManager manages all VAOs generated and their VBOs.
 * VAOs are identified via unique string
 */
#ifndef VAOSMANAGER_GRAPHICS_JPL
#define VAOSMANAGER_GRAPHICS_JPL

#include <unordered_map>
#include "VAO.hpp"

namespace jpl{
    namespace _graphics{
        namespace _engine{

            class VAOManager{


                protected:

                    std::unordered_map<std::string, VAO* const> vaos;

                public:

                    VAOManager();
                    ~VAOManager();

                    /**
                     * @throw IllegalArgumentException if vaovbo is nullptr
                     * @throw RuntimeException if another vao with the same name already exists
                     */
                    VAO* const addNewVAO(const std::string& identifier, VAO* const vao);
                    /**
                     * @throw RuntimeException if another vao with the same name already exists
                     */
                    VAO* const addNewVAO(const std::string &identifier, const unsigned int VAO);

                    /**
                     * @param name
                     * @throw RuntimeException if another vao with the same name already exists
                     * @return new VAO created
                     */
                    VAO* const addNewVAO(const std::string &identifier);

                    /**
                     * @return VAO with given name otherwise nullptr
                     */
                    VAO* const getVAOByName(const std::string &identifier) const noexcept;

                    /**
                     * @param identifier
                     * @throw IllegalArgumentException if no VAO with the given name has been found
                     */
                    void removeVAOByName(const std::string &identifier);

                    static VAOManager* INSTANCE;
                    static void initializeVAOManager();
            };
        }
    }
}

#endif