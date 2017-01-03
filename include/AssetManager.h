#pragma once

#include "cinder/gl/Texture.h"
#include "cinder/gl/VboMesh.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/audio/Voice.h"

#include <string>
#include <vector>

namespace am // am -> asset manager
{
    ci::SurfaceRef& surface(const std::string& relativeName);
    ci::ChannelRef& channel(const std::string& relativeName);
    ci::Channel16uRef& channel16u(const std::string& relativeName);

    // Supports jpg, png, bmp, tga, dds, ktx, hdr etc.
    // Special support: "checkerboard"
    //ci::gl::Texture1dRef& texture1d(const std::string& relativeName, const ci::gl::Texture1d::Format& format = ci::gl::Texture1d::Format());
    ci::gl::Texture2dRef& texture2d(const std::string& relativeName,
                                    const ci::gl::Texture2d::Format& format = ci::gl::Texture2d::Format()
                                        .mipmap().minFilter(GL_LINEAR_MIPMAP_LINEAR).magFilter(GL_LINEAR));
    ci::gl::TextureCubeMapRef& textureCubeMap(const std::string& relativeName,
                                              const ci::gl::TextureCubeMap::Format& format = ci::gl::TextureCubeMap::Format()
                                              .mipmap().minFilter(GL_LINEAR_MIPMAP_LINEAR).magFilter(GL_LINEAR));

    // Supports obj, msh.
    // Special support: "Rect", "Icosahedron", "Icosphere", "Teapot", "Circle", "Ring", "Sphere", "Capsule" etc
    ci::TriMeshRef& triMesh(const std::string& relativeName);

    ci::gl::VboMeshRef& vboMesh(const std::string& relativeName);

    // Special support of vsFileName: "color", "texture", "lambert", "color+texture"
    ci::gl::GlslProgRef& glslProg(const std::string& vsFileName, const std::string& fsFileName = "");

    std::string& str(const std::string& relativeName);

    std::vector<std::string>& longPaths(const std::string& relativeFolderName);
    std::vector<std::string>& shortPaths(const std::string& relativeFolderName);

    ci::audio::VoiceRef voice(const std::string& relativeName);
}
