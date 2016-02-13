#pragma once

#include "cinder/gl/Texture.h"
#include "cinder/gl/VboMesh.h"
#include "cinder/gl/GlslProg.h"
#include <string>
#include <vector>

namespace am // am -> asset manager
{
    ci::SurfaceRef& surface(const std::string& relativeName);

    ci::gl::TextureRef& texture(const std::string& relativeName);

    ci::TriMeshRef& triMesh(const std::string& relativeName);

    ci::gl::VboMeshRef& vboMesh(const std::string& relativeName);

    ci::gl::GlslProgRef& glslProg(const std::string& vsFileName, const std::string& fsFileName);

    std::string& str(const std::string& relativeName);

    std::vector<std::string> files(const std::string& relativeFolderName);
}
