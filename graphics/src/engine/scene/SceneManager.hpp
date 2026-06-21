/**
 * SceneManager manages all scenes via a pointer to the current scene that has to be rendered
 */

#ifndef SCENE_MANAGER_GRAPHICS_JPL
#define SCENE_MANAGER_GRAPHICS_JPL

#include <unordered_map>
#include <string>
#include <jpl/exception/runtime/IllegalArgumentException.hpp>
#include <jpl/logger/Logger.hpp>
#include "Scene.hpp"

namespace jpl{
    namespace _graphics{
        namespace _engine{

                class SceneManager{

                    protected:
                        std::unordered_map<std::string, Scene*> scenes;

                        std::string currentSceneName;
                        Scene* currentScene;

                    public:

                        SceneManager();

                        /**
                         * @param newSceneName name of the new scene
                         * @param newScene new scene
                         * @throw IllegalArgument if newScene is nullptr
                         * @throw RuntimeException if a scene with the same newSceneName is already present in the map
                         */
                        virtual void addNewScene(const std::string &newSceneName, Scene* newScene);

                        /**
                         * @param name
                         * @throw RuntimeException if there's no scene with the given name
                         */
                        virtual void removeSceneByName(const std::string &name);

                        /**
                         * @param name
                         * @return scene with the given name otherwise nullptr
                         */
                        virtual Scene* const getSceneByName(const std::string& name) const;
                        /**
                         * @param name
                         * @throw IllegalArgument if there's no scene with the given name
                         */
                        virtual void setNewCurrentSceneByName(const std::string &name);

                        /**
                         * @return current scene name; if no scene is set, it returns an empty string
                         */
                        virtual std::string getCurrentSceneName() const noexcept;

                        /**
                         * @return current scene or nullptr if no scene is set
                         */
                        virtual Scene* const getCurrentScene() const noexcept;

                        /**
                         * Set to nullptr current scene in order to prevent any scene to be rendered.
                         * it also clean current scene name
                         */
                        virtual void disable() noexcept;

                        /**
                         * Render current scene
                         * it does not check if currentScene is nullptr, therefore undefined behaviour may occurr
                         */
                        virtual void render();

                        static SceneManager* INSTANCE;
                        static void initializeSceneManager();

                        
                        ~SceneManager();
                };

        }
    }
}

#endif