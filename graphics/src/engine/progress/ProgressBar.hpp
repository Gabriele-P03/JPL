/**
 * A progress bar is a widget which represent a percentage bar dinamically filled.
 * Filled Texture part must be always drawn above the empty one.
 * 
 * It is up to the programmer to set floating units value for VBO.
 * Just remember that indices are fixed as described below:
 * 0,1,2
 * 0,1,3
 * Where 0 is bottom-left, 1 is top-right, 2 is top-left and 3 is bottom-right 
 * 
 * In order to avoid overhead due to two call to glDrawElements (one for empty part and one for filled one), both vertices are stored into a single float array and passed 
 * to BufferData via single call 
 */
#ifndef PROGRESSBAR_GRAPHICS_JPL
#define PROGRESSBAR_GRAPHICS_JPL

#include "../Painter.hpp"
#include "../text/TextRender.hpp"

namespace jpl{
    namespace _graphics{
        namespace _engine{

            class ProgressBar{

                protected:

                    _texture::Texture* background;

                    float progress, max;

                    static constexpr int indicesPerPart[2] = {6,6};
                    static constexpr int indices[12] = {
                        0,1,2,  //Empty part
                        0,1,3,

                        4,5,6,  //Filled part
                        4,5,7
                    };

                public:

                    /**
                     * @param painter
                     * @param textRender
                     * @param texture
                     * @param max max value of the progress bar
                     * @param xSep x coord of the texture where filled part begins
                     * @param ySep y coord of the texture where filled part begins
                     * @param w width of the filled part
                     * @param h height of the filled part
                     * @throw IllegalArgumentException if either painter or texture are nullptr 
                     */
                    ProgressBar(_texture::Texture* background, float max);


                    float getProgress() const noexcept{
                        return this->progress;
                    }
                    void setMax(float max) noexcept{
                        this->max = max;
                    }
                    float getMax() const noexcept{
                        return this->max;
                    }

                    /**
                     * @param p
                     * @throw IllegalArgumentException if p is less than 0 or greater than max
                     */
                    virtual void setProgress(float p);

                    virtual void render() const noexcept;

                    
            };

        }
    }
}

#endif