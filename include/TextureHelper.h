#pragma once

#include "cinder/gl/Texture.h"

using namespace ci;

template <typename T>
void updateTexture(gl::TextureRef &tex, const T &src)
{
    if (src.getWidth() == 0) return;

    if (!tex)
    {
        tex = gl::Texture2d::create(src);
    }
    else
    {
        tex->update(src);
    }
}
