/**
 * An object which extends this interface is able to be clicked (e.g. buttons and textrender).
 * A renderable object should be renderable as well, therefore IRenderable is extended  
 */

#ifndef ICLICKABLE_GRAPHICS_JPL
#define ICLICKABLE_GRAPHICS_JPL

#include <functional>


namespace jpl{
    namespace _graphics{
        namespace _engine{

            class IClickable{

                protected:
                    float x,y,w,h;
                    bool focus;
                    IClickable(float x, float y, float w, float h) : x(x), y(y), w(w), h(h), focus(false){}

                    std::function<void()> f;

                public:
                    virtual ~IClickable() = default;

                    float getX(){ return x; }
                    float getY(){ return y; }
                    float getW(){ return w; }
                    float getH(){ return h; }

                    virtual void setOnClick( std::function<void()> f_ ) noexcept{
                        this->f = std::move(f_);
                    }

                    virtual void setFocus( bool focus_ ){ this->focus = focus_; }
                    bool isFocused() const noexcept{ return this->focus; }

                    virtual void click(){
                        this->f();
                    }
            };
        }
    }
}

#endif