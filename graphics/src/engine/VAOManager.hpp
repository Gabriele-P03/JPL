/**
 * VAOsManager manages all VAOs generated and their VBOs.
 * VAOs are identified via unique string
 */
#ifndef VAOSMANAGER_GRAPHICS_JPL
#define VAOSMANAGER_GRAPHICS_JPL

#include "VAO.hpp"

namespace jpl{
    namespace _graphics{
        namespace _engine{

            class VAOManager{


                protected:

                    std::vector<VAO*> list;

                public:

                    VAOManager() = default;
                    ~VAOManager() = default;

                    /**
                     * @throw IllegalArgumentException if vaovbo is nullptr
                     * @throw RuntimeException if another vao with the same name already exists
                     */
                    void addNewVAO(VAO* vao);
                    /**
                     * @throw RuntimeException if another vao with the same name already exists
                     */
                    void addNewVAO(const std::string &name, unsigned int VAO);

                    /**
                     * @param name
                     * @throw RuntimeException if another vao with the same name already exists
                     * @return new VAO created
                     */
                    VAO* addNewVAO(const std::string &name);

                    /**
                     * @return VAO with given name or nullptr
                     */
                    VAO* getVAOByName(const std::string &name) const noexcept;
            };
        }
    }
}

#endif