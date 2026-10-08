#pragma once

#include <string>

#include "Font.h"

namespace jpl::_graphics::_engine::_text {

    /**
     * Load TTF from path
     * @param out
     * @param path
     * @param pixelHeight
     * @param atlasSize
     */
    extern void loadFontTTF(jpl::_graphics::_engine::_text::Font& out, const std::string &path, float pixelHeight, int atlasSize = 512);

    extern void destroyFont(jpl::_graphics::_engine::_text::Font& f);
}

