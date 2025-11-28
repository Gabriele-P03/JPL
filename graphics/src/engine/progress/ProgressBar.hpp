/**
 * A progress bar is a widget which represent a percentage bar dinamically filled.
 * Filled Texture part must be always drawn above the empty one 
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
                    float emptyCoords[20], filledCoords[20];


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
                    ProgressBar(_engine::Painter* painter, _engine::_text::TextRender* textRender, _texture::Texture* background, float max, unsigned int xSep, unsigned int ySep, unsigned int w, unsigned int h);

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
                    void setCoordsArray(float empty[20], float filled[20]) noexcept{
                        this->emptyCoords = empty;
                        this->filledCoords = filled;
                    }
                    
            };

        }
    }
}

#endif