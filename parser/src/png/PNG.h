/**
 * This is a PNG parser based on stb library, but you can use it for any other image format available for stb
 */
#ifndef PNG_JPL
#define PNG_JPL

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"
#include <string>
#include <jpl/exception/runtime/RuntimeException.hpp>

namespace jpl{
    namespace _parser{
        namespace _png{

            /**
             * You have to free memory by yourself via stbi_image_free(const char*)
             * @param path to the file
             * @param w - out
             * @param h - out
             * @param channels - out
             * @param expectedChannels - in
             * 
             * @return pointer to the image data
             */
            unsigned char* load_image(const std::string& path, int* w, int* h, int* channels, int expectedChannels);
        }
    }
}

#endif