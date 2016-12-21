#pragma once

#include "cinder/gl/Texture.h"

using namespace ci;

template <typename T>
void updateTexture(gl::TextureRef &tex, const T &src, const gl::Texture2d::Format& format = gl::Texture2d::Format())
{
    if (src.getWidth() == 0) return;

    if (!tex 
        || tex->getWidth() != src.getWidth()
        || tex->getHeight() != src.getHeight())
    {
        tex = gl::Texture2d::create(src, format);
    }
    else
    {
        tex->update(src);
    }
}
