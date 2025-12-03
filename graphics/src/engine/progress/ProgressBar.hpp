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
                    _engine::Painter* painter;
                    _engine::_text::TextRender* textRender;

                    float progress;
                    float max;

                    /**
                     * They contains coords of the empty progress bar part in the texture and filled one
                     */
                    float coords[40];

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
                    ProgressBar(_engine::Painter* painter, _engine::_text::TextRender* textRender, _texture::Texture* background, float max);

                    float getProgress() const noexcept{
                        return this->progress;
                    }
                    float getMax() const noexcept{
                        return this->max;
                    }

                    /**
                     * @param p
                     * @throw IllegalArgumentException if p is less than 0 or greater than max
                     */
                    virtual void setProgress(float p){
                        if(p <= 0){
                            throw jpl::_exception::IllegalArgumentException("progress " + std::to_string(p) + " is less than 0");
                        }
                        if(p > this->max){
                            throw jpl::_exception::IllegalArgumentException("progress " + std::to_string(p) + " is less than max " + std::to_string(this->max));
                        }
                        this->progress = p;
                    }

                    virtual void render() const noexcept;
                    /**
                     * It sets float coords array of the empty and filled part both
                     * @param empty
                     * @param filled
                     */
                    void setCoordsArray(std::array<float, 40> const &coords) noexcept{
                        std::copy(coords.begin(), coords.begin()+40, this->coords);
                    }
                    
            };

        }
    }
}

#endif