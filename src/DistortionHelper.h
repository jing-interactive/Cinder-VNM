//
//  OculusVR.h
//  OculusSDKTest
//
//  Created by Simon Geilfus on 29/05/13.
//
//

#pragma once


#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"

// Distortion Shader Class
typedef std::shared_ptr<class DistortionHelper> DistortionHelperRef;

class DistortionHelper
{
public:
    //! Returns a shared_ptr DistortionHelper
    static DistortionHelperRef create( bool chromaticAbCorrection = true );
    
    //! Returns fullscreen quad with both distorted eyes from a gl::TextureRef
    void render( const ci::gl::TextureRef &texture, const ci::Rectf &rect = ci::Rectf( ci::vec2(0,0), ci::vec2(1280,800) ) );
    //! Returns fullscreen quad with both distorted eyes from a gl::Texture
    void render( const ci::gl::Texture &texture, const ci::Rectf &rect = ci::Rectf( ci::vec2(0,0), ci::vec2(1280,800) )  );
    
protected:
    DistortionHelper( bool chromaticAbCorrection = true );
    
    ci::vec4           mDistortionParams;
    float               mDistortionScale;
    
    bool                mUseChromaticAbCorrection;
    ci::vec4           mChromaticAbCorrection;
    ci::gl::GlslProgRef mShader;
};


