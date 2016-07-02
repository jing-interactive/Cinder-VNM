#pragma once

#include "cinder/gl/GlslProg.h"
#include "cinder/params/Params.h"

#include <unordered_map>

struct GlslParamsBuilder
{
    GlslParamsBuilder() {}
    
    template <typename T>
    ci::params::InterfaceGl::Options<T> addParam(ci::gl::GlslProgRef glsl, ci::params::InterfaceGlRef params, std::string name, T& value)
    {
        return params->addParam(name, &value).updateFn([glsl, name, &value]{
            glsl->uniform(name, value);
        });
    }
    
    GlslParamsBuilder(ci::gl::GlslProgRef glsl, ci::params::InterfaceGlRef params)
    {
        glslProg = glsl;
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
                    addParam(glsl, params, name, namedFloats[name]).step(0.1f);
                    break;
                }
                case GL_FLOAT_VEC3:
                {
                    addParam(glsl, params, name, namedColors[name]); // namedColors or namedVec3s?
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
    
    void applyUniforms()
    {
        for (auto& kv : namedInts) glslProg->uniform(kv.first, kv.second);
        for (auto& kv : namedBools) glslProg->uniform(kv.first, kv.second);
        for (auto& kv : namedFloats) glslProg->uniform(kv.first, kv.second);
        for (auto& kv : namedVec3s) glslProg->uniform(kv.first, kv.second);
        for (auto& kv : namedColors) glslProg->uniform(kv.first, kv.second);
        for (auto& kv : namedColorAs) glslProg->uniform(kv.first, kv.second);
    }
    
    ci::gl::GlslProgRef glslProg;
    
    std::unordered_map<std::string, int> namedInts;
    std::unordered_map<std::string, bool> namedBools;
    std::unordered_map<std::string, float> namedFloats;
    std::unordered_map<std::string, ci::vec3> namedVec3s;
    std::unordered_map<std::string, ci::Color> namedColors;
    std::unordered_map<std::string, ci::ColorA> namedColorAs;
};

