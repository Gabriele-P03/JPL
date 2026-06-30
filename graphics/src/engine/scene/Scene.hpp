/**
 * A scene represents a set of correlated objects (i.e. in the same context) that must be rendered all together.
 * An example of scene may be the main menu
 * 
 * 
 * buttons list is declared as const in order to let programmer choose to use it, since there are
 * several ways to manage clicks basing on context (i.e. 2D/3D or 1000+ buttons) 
 * 
 */
#ifndef SCENE_GRAPHICS_JPL
#define SCENE_GRAPHICS_JPL

#include <unordered_map>
#include "../button/Button.hpp"

namespace jpl{
    namespace _graphics{
        namespace _engine{

            class Scene{

                protected:

                    std::vector<_button::Button*>* buttons;

                public:
                    Scene();
                    virtual ~Scene();

                    /**
                     * Thias callback is set everytime a scene is set as current
                     * @param window
                     * @param button
                     * @param action
                     * @param mods
                     */
                    virtual void clickCallback(GLFWwindow* window, int button, int action, int mods);


                    virtual void render() = 0;

                    /**
                     * This function could help you to push VBOs and EBOs data.
                     * It is up to you to recognize where and when call it.
                     * Scene class does not include VAOs on its own, therefore you need to specify it in your class
                     */
                    virtual void pushData() = 0;
            };
            
        }
    }
}

#endif