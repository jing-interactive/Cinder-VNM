#pragma once

#include "cinder/gl/TextureFont.h"

struct FontHelper
{
    static const std::vector<std::string>& getFontNames()
    {
        return ci::Font::getNames();
    }

    static ci::gl::TextureFontRef createTextureFont(const std::string& fontName = "Times New Roman", float fontScale = 24)
    {
        auto font = ci::Font(fontName, fontScale);
        return ci::gl::TextureFont::create(font, gl::TextureFont::Format().premultiply());
    }
};
