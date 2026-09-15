/**
 * An object which extends this interface is able to be rendered (e.g. buttons and textrender).
 * This interface must be extended only by those objects which are intended to be 2D one 
 */

#ifndef IRENDERABLE_GRAPHICS_JPL
#define IRENDERABLE_GRAPHICS_JPL

#include "../Painter.hpp"

namespace jpl{
    namespace _graphics{
        namespace _engine{

            class IRenderable{

                protected:
                    float x,y,w,h;

                public:

                    IRenderable(float x, float y, float w, float h){
                        this->setDim(x,y,w,h);
                    }
                    
                    virtual void setDim(float x, float y, float w, float h){
                        this->x = x;
                        this->y = y;
                        this->w = w;
                        this->h = h;
                    }

                    float getX() const noexcept{
                        return this->x;
                    }
                    float getY() const noexcept{
                        return this->y;
                    }
                    float getW() const noexcept{
                        return this->w;
                    }
                    float getH() const noexcept{
                        return this->h;
                    }

                    virtual void render(Painter* painter) = 0;
            };
        }
    }
}


#endif