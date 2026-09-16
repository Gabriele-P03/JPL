/**
 * A scene represents a set of correlated objects (i.e. in the same context) that must be rendered all together.
 * An example of scene may be the main menu
 * 
 * 
 * 
 */
#ifndef SCENE_GRAPHICS_JPL
#define SCENE_GRAPHICS_JPL

#include <unordered_map>
#include "../button/Button.hpp"
#include "../text/TextRender.hpp"

namespace jpl{
    namespace _graphics{
        namespace _engine{

            class Scene{

                protected:

                    std::vector<IClickable*> clickables;
                    
                    //It is the last clicked element
                    IClickable* focusedElement;

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

                    virtual void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

                    virtual void charCallback(GLFWwindow* window, unsigned int codepoint);


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