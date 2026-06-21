#include "png.h"

unsigned char* jpl::_parser::_png::load_image(const std::string& path, int* w, int* h, int* channels, int expectedChannels){
    unsigned char* data = stbi_load(path.c_str(), w, h, channels, expectedChannels);
    if(data == nullptr){
        throw jpl::_exception::RuntimeException("Image " + path + " could not be loaded: " + stbi_failure_reason());
    }
    return data;
}