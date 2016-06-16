//
//  OculusVR.cpp
//  OculusSDKTest
//
//  Created by Simon Geilfus on 29/05/13.
//
//

#include "../include/DistortionHelper.h"

#include "cinder/Utilities.h"
#include "cinder/app/App.h"
#include "cinder/gl/Context.h"
#include "cinder/gl/Environment.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/Vao.h"
#include "cinder/gl/scoped.h"

using namespace ci;

static const char* GenericFragShaderSrc =
//"#version 150\n"
"precision highp float;"
"\n"
//"out vec4 oColor;\n"
"uniform sampler2D uTex0;\n"
"varying vec2	TexCoord;\n"
"void main( void )\n"
"{\n"
"    gl_FragColor = texture( uTex0, TexCoord.st );\n"
"}";

static const char* GenericVertShaderSrc =
//"#version 150\n"
"\n"
"uniform mat4	ciModelViewProjection;\n"
"\n"
"attribute vec4		ciPosition;\n"
"attribute vec2		ciTexCoord0;\n"
"varying highp vec2	TexCoord;\n"
"\n"
"void main( void )\n"
"{\n"
"    gl_Position	= ciModelViewProjection * ciPosition;\n"
"    TexCoord	= ciTexCoord0;\n"
"}\n";

static const char* PostProcessFragShaderSrc =
//"#version 150\n"
"precision highp float;"
"uniform vec2 LensCenter;\n"
"uniform vec2 ScreenCenter;\n"
"uniform vec2 Scale;\n"
"uniform vec2 ScaleIn;\n"
"uniform vec4 HmdWarpParam;\n"
"uniform sampler2D uTex0;\n"
"\n"
"varying vec2 TexCoord;\n"
//"out vec4 oColor;\n"
"\n"
"vec2 HmdWarp(vec2 in01)\n"
"{\n"
"   vec2  theta = (in01 - LensCenter) * ScaleIn;\n" // Scales to [-1, 1]
"   float rSq = theta.x * theta.x + theta.y * theta.y;\n"
"   vec2  theta1 = theta * (HmdWarpParam.x + HmdWarpParam.y * rSq + "
"                           HmdWarpParam.z * rSq * rSq + HmdWarpParam.w * rSq * rSq * rSq);\n"
"   return LensCenter + Scale * theta1;\n"
"}\n"
"void main( void )\n"
"{\n"
"   vec2 tc = HmdWarp(TexCoord.st);\n"
"   if (!all(equal(clamp(tc, ScreenCenter-vec2(0.25,0.5), ScreenCenter+vec2(0.25,0.5)), tc)))\n"
"       gl_FragColor = vec4(0,0,0,1);\n"
"   else\n"
"       gl_FragColor = texture2D(uTex0, tc);\n"
"}\n";


// Shader with lens distortion and chromatic aberration correction.
static const char* PostProcessFullFragShaderSrc =
//"#version 150\n"
"precision highp float;"
"uniform vec2 LensCenter;\n"
"uniform vec2 ScreenCenter;\n"
"uniform vec2 Scale;\n"
"uniform vec2 ScaleIn;\n"
"uniform vec4 HmdWarpParam;\n"
"uniform vec4 ChromAbParam;\n"
"uniform sampler2D uTex0;\n"
"\n"
"varying vec2 TexCoord;\n"
//"out vec4 oColor;\n"
"\n"
// Scales input texture coordinates for distortion.
// ScaleIn maps texture coordinates to Scales to ([-1, 1]), although top/bottom will be
// larger due to aspect ratio.
"void main()\n"
"{\n"
"   vec2  theta = (TexCoord.st - LensCenter) * ScaleIn;\n" // Scales to [-1, 1]
"   float rSq= theta.x * theta.x + theta.y * theta.y;\n"
"   vec2  theta1 = theta * (HmdWarpParam.x + HmdWarpParam.y * rSq + "
"                  HmdWarpParam.z * rSq * rSq + HmdWarpParam.w * rSq * rSq * rSq);\n"
"   \n"
"   // Detect whether blue texture coordinates are out of range since these will scaled out the furthest.\n"
"   vec2 thetaBlue = theta1 * (ChromAbParam.z + ChromAbParam.w * rSq);\n"
"   vec2 tcBlue = LensCenter + Scale * thetaBlue;\n"
"   if (!all(equal(clamp(tcBlue, ScreenCenter-vec2(0.25,0.5), ScreenCenter+vec2(0.25,0.5)), tcBlue)))\n"
"   {\n"
"       gl_FragColor = vec4(0);\n"
"       return;\n"
"   }\n"
"   \n"
"   // Now do blue texture lookup.\n"
"   float blue = texture2DuTex0, tcBlue).b;\n"
"   \n"
"   // Do green lookup (no scaling).\n"
"   vec2  tcGreen = LensCenter + Scale * theta1;\n"
"   vec4  center = texture2D(uTex0, tcGreen);\n"
"   \n"
"   // Do red scale and lookup.\n"
"   vec2  thetaRed = theta1 * (ChromAbParam.x + ChromAbParam.y * rSq);\n"
"   vec2  tcRed = LensCenter + Scale * thetaRed;\n"
"   float red = texture2D(uTex0, tcRed).r;\n"
"   \n"
"   gl_FragColor = vec4(red, center.g, blue, 1);\n"
"}\n";


DistortionHelperRef DistortionHelper::create( bool chromaticAbCorrection )
{
    return DistortionHelperRef( new DistortionHelper( chromaticAbCorrection ) );
}
DistortionHelper::DistortionHelper( bool chromaticAbCorrection )
:
mDistortionParams( 1,0.22,0.24,0 ),
mDistortionScale( 1.71461f ),
mChromaticAbCorrection( 0.996, -0.004, 1.014, 0 ),
mUseChromaticAbCorrection( chromaticAbCorrection )
{
    
    // Load and compile Distortion Shader
    try
    {
        mShader = gl::GlslProg::create( GenericVertShaderSrc,
                                       chromaticAbCorrection ?
                                       PostProcessFullFragShaderSrc :
                                       PostProcessFragShaderSrc );
    }
    catch( gl::GlslProgCompileExc exc )
    {
        std::cout << "ovr::DistortionHelper Exception: " << std::endl << exc.what() << std::endl;
    }
}

void DistortionHelper::render( const gl::TextureRef &texture, const Rectf &rect )
{
    render( *texture, rect );
}

void DistortionHelper::render( const gl::Texture &texture, const Rectf &rect )
{
    float scale = ci::app::App::get()->isHighDensityDisplayEnabled() ? 2.f : 1.f;
    
    float w                         = 0.5f;
    float h                         = 1.0f;
    float x                         = 0.0f;
    float y                         = 0.0f;
    float as                        = ( (float) rect.getWidth() * 0.5f ) / (float) rect.getHeight();
    float distortionXCenterOffset   = 0.25f / mDistortionScale;
    float scaleFactor               = 1.0f / mDistortionScale;
    
    {
        auto ctx = gl::context();
        
        gl::ScopedVao vaoScp( ctx->getDrawTextureVao() );
        gl::ScopedBuffer vboScp( ctx->getDrawTextureVbo() );
        gl::ScopedGlslProg glslScp( mShader );
        
        // Translate / scale
        gl::ScopedMatrices scaleVbo;
        gl::scale( rect.getSize() );
        
        // Set the uniforms
        mShader->uniform( "uTex0", 0 );
        mShader->uniform( "ciModelViewProjection", gl::getModelViewProjection() );
        
        mShader->uniform( "LensCenter", vec2( x + (w + distortionXCenterOffset * 0.5f)*0.5f, y + h*0.5f ) );
        mShader->uniform( "ScreenCenter", vec2( x + w*0.5f, y + h*0.5f ) );
        
        mShader->uniform( "Scale", vec2( (w/2) * scaleFactor, (h/2) * scaleFactor * as ) );
        mShader->uniform( "ScaleIn", vec2( (2/w),               (2/h) / as ) );
        mShader->uniform( "HmdWarpParam", mDistortionParams );
        
        if( mUseChromaticAbCorrection )
        {
            mShader->uniform( "ChromAbParam", mChromaticAbCorrection );
        }
        
        texture.bind(0);
        
        // Draw left
        glEnable( GL_SCISSOR_TEST );
        glScissor( 0, 0, rect.getWidth() * 0.5 * scale, rect.getHeight() * scale);
        ctx->drawArrays( GL_TRIANGLE_STRIP, 0, 4 );
        
        // Move the cam
        distortionXCenterOffset = -0.25f / mDistortionScale;
        scaleFactor             = 1.0f / mDistortionScale;
        x                       = 0.5f;
        
        mShader->uniform( "LensCenter", vec2( x + (w + distortionXCenterOffset * 0.5f)*0.5f, y + h*0.5f ) );
        mShader->uniform( "ScreenCenter", vec2( x + w*0.5f, y + h*0.5f ) );
        mShader->uniform( "Scale", vec2( (w/2) * scaleFactor, (h/2) * scaleFactor * as ) );
        
        // Draw right
        glScissor( rect.getWidth() * 0.5 * scale, 0, rect.getWidth() * scale, rect.getHeight() * scale);
        ctx->drawArrays( GL_TRIANGLE_STRIP, 0, 4 );
        glDisable( GL_SCISSOR_TEST );
        
        texture.unbind();
    }
}

