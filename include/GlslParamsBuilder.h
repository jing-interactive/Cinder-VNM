#pragma once

#include "cinder/gl/GlslProg.h"
#include "cinder/params/Params.h"

#include <unordered_map>

struct GlslParamsBuilder
{
    GlslParamsBuilder() {}
    
    template <typename T>
    void addParam(ci::gl::GlslProgRef glsl, ci::params::InterfaceGlRef params, std::string name, T& value)
    {
        params->addParam(name, &value).updateFn([glsl, name, &value]{
            glsl->uniform(name, value);
        });
    }
    
    GlslParamsBuilder(ci::gl::GlslProgRef glsl, ci::params::InterfaceGlRef params)
    {
        //
        // glsl reflection: uniform
        //
        auto activeUniforms = glsl->getActiveUniforms();
        for (auto uniform : activeUniforms)
        {
            if (uniform.getCount() != 1) continue; // skip array
            if (uniform.getUniformSemantic() != ci::gl::UNIFORM_USER_DEFINED) continue; // skip cinder-defined semantic
            auto name = uniform.getName();
            
            switch (uniform.getType())
            {
                case GL_INT:
                case GL_SAMPLER_2D:
                case GL_SAMPLER_CUBE:
                {
                    addParam(glsl, params, name, namedInts[name]);
                    break;
                }
                case GL_FLOAT:
                {
                    addParam(glsl, params, name, namedFloats[name]);
                    break;
                }
                case GL_FLOAT_VEC3:
                {
                    addParam(glsl, params, name, namedVec3s[name]);
                    break;
                }
                case GL_BOOL:
                {
                    addParam(glsl, params, name, namedBools[name]);
                    break;
                }
                default:
                {
                    params->addText(name);
                    break;
                }
            }
        }
   
    }
private:
    
    std::unordered_map<std::string, int> namedInts;
    std::unordered_map<std::string, bool> namedBools;
    std::unordered_map<std::string, float> namedFloats;
    std::unordered_map<std::string, ci::vec3> namedVec3s;

};

