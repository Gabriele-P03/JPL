/**
 * An object which extends this interface is able to be clicked (e.g. buttons and textrender).
 * A renderable object should be renderable as well, therefore IRenderable is extended  
 */

#ifndef ICLICKABLE_GRAPHICS_JPL
#define ICLICKABLE_GRAPHICS_JPL

#include <functional>
#include "IRenderable.hpp"


namespace jpl{
    namespace _graphics{
        namespace _engine{

            class IClickable : public IRenderable{

                protected:

                    IClickable(float x, float y, float w, float h) : IRenderable(x,y,w,h){}

                    std::function<void()> f;

                public:

                    virtual void setOnClick( std::function<void()> f ) noexcept{
                        this->f = std::move(f);
                    }

                    virtual void click(){
                        this->f();
                    }
            };
        }
    }
}

#endif