#include "Font.hpp"

jpl::_graphics::_engine::_text::Font::Font(
    std::string pathToFontMap, CHARSET charset, unsigned int charsPerWidth, unsigned int charsPerHeight, unsigned int chars
){
    std::fstream* f = new std::fstream;
    jpl::_utils::_files::getInternalFile("fonts/"+pathToFontMap, std::ios_base::in | std::ios_base::binary, &f);
    jpl::_parser::_bmp::BMP* bmp = jpl::_parser::_bmp::read(f);
    size_t s = 0;
    char* buffer = jpl::_parser::_bmp::toRender(bmp, s);
    this->texture = new jpl::_graphics::_texture::Texture(
        bmp->width, bmp->height, 3, buffer
    );
    this->charset = charset;
    this->chars = chars;
    this->charsPerHeight = charsPerHeight;
    this->charsPerWidth = charsPerWidth;
    this->texture->bind();
    this->texture->generate();
}

jpl::_graphics::_engine::_text::Font::Font(jpl::_graphics::_texture::Texture* texture, CHARSET charset, unsigned int charsPerWidth, unsigned int charsPerHeight, unsigned int chars){
    this->texture = texture;
    this->charset = charset;
    this->chars = chars;
    this->charsPerHeight = charsPerHeight;
    this->charsPerWidth = charsPerWidth;
    this->texture->bind();
    this->texture->generate();
}


jpl::_graphics::_engine::_text::Font* jpl::_graphics::_engine::_text::ARIALS = nullptr;