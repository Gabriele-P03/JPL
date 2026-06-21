/**
 * A scene represents a set of correlated objects (i.e. in the same context) that must be rendered all together.
 * An example of scene may be the main menu
 * 
 * 
 */
#ifndef SCENE_GRAPHICS_JPL
#define SCENE_GRAPHICS_JPL

namespace jpl{
    namespace _graphics{
        namespace _engine{

            class Scene{
                public:
                    Scene(){}
                    virtual ~Scene() = default;
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