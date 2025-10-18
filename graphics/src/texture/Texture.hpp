/**
 * 
 */

#ifndef TEXTURE_GRAPHICS_JPL
#define TEXTURE_GRAPHICS_JPL

#include <GL/glew.h>
#include <GL/gl.h>

namespace jpl{
    namespace _graphics{
        namespace _texture{

            class Texture{

                protected:
                    unsigned int textureIndex;

                    const unsigned int width;
                    const unsigned int height;
                    const unsigned int nrChannels;

                    const char* data;

                    //GL_RGBA or GL_RGB
                    const unsigned int format;
                    //GL_TEXTURE_1D or GL_TEXTURE_2D or GL_TEXTURE_3D 
                    const unsigned int type;

                public:
                
                    Texture(unsigned int width, unsigned int height, unsigned int nrChannels, const char* data, unsigned int format, unsigned int type)
                        : width(width), height(height), nrChannels(nrChannels), data(data), format(format), type(type){

                        glGenTextures(1, &this->textureIndex);

                    }
                    Texture(unsigned int width, unsigned int height, unsigned int nrChannels, const char* data)
                        : Texture(width, height, nrChannels, data, GL_RGB, GL_TEXTURE_2D){

                    }

                    virtual void bind() const noexcept{
                        glBindTexture(this->type, this->textureIndex);
                    }

                    virtual void generate(unsigned int level = 0) const noexcept{
                        glTexImage2D(this->type, level, this->format, this->width, this->height, 0, this->format, GL_UNSIGNED_BYTE, this->data);
                        glGenerateMipmap(this->type);
                    }

                    unsigned int getTextureIndex() const noexcept{
                        return this->textureIndex;
                    }
                    unsigned int getWidth() const noexcept{
                        return this->width;
                    }
                    unsigned int getHeight() const noexcept{
                        return this->height;
                    }
                    unsigned int getNrChannels() const noexcept{
                        return this->nrChannels;
                    }
                    unsigned int getFormat() const noexcept{
                        return this->format;
                    }
                    unsigned int getType() const noexcept{
                        return this->type;
                    }
                    const char* getData() const noexcept{
                        return this->data;
                    } 
            };

        }
    }
}

#endif