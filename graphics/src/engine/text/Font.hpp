/**
 * 
 */

#ifndef FONT_GRAPHICS_JPL
#define FONT_GRAPHICS_JPL

#include <string>
#include "../../texture/Texture.hpp"

#include <jpl/parser/bmp/BMP.hpp>
#include <jpl/utils/FilesUtils.hpp>

namespace jpl{
    namespace _graphics{
        namespace _engine{
            namespace _text{

                enum CHARSET{
                    UNICODE,
                    ASCII,
                    BASE64,
                    UTF8
                };

                class Font{

                    protected:

                        unsigned int charsPerWidth;
                        unsigned int charsPerHeight;
                        unsigned int chars;

                        CHARSET charset;

                        _texture::Texture* texture;

                    public:

                        Font(std::string pathToFontMap, CHARSET charset, unsigned int charsPerWidth, unsigned int charsPerHeight, unsigned int chars);
                        Font(_texture::Texture* texture, CHARSET charset, unsigned int charsPerWidth, unsigned int charsPerHeight, unsigned int chars);

                        const _texture::Texture* getTexture() const noexcept{
                            return this->texture;
                        }

                        unsigned int getWidthFontMap() const noexcept{
                            return this->texture->getWidth();
                        }
                        unsigned int getHeightFontMap() const noexcept{
                            return this->texture->getHeight();
                        }

                        CHARSET getCharset() const noexcept{
                            return this->charset;
                        } 

                        unsigned int getCharsPerWidth() const noexcept{
                            return this->charsPerWidth;
                        }

                        unsigned int getCharsPerHeight() const noexcept{
                            return this->charsPerHeight;
                        }

                        unsigned int getCharsAmount() const noexcept{
                            return this->chars;
                        }

                        unsigned int getPixelWidthPerChar() const noexcept{
                            return this->texture->getWidth()/this->charsPerWidth;
                        }
                        unsigned int getPixelHeightPerChar() const noexcept{
                            return this->texture->getHeight()/this->charsPerHeight;
                        }

                };

                extern Font* ARIALS;
                //extern const Font* const ITALIC;
                //extern const Font* const BOLD;
                //extern const Font* const UNDERLINE;
                
            }
        }
    }
}

#endif