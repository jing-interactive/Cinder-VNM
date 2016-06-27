#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/CameraUi.h"
#include "cinder/params/params.h"

#include "AssetManager.h"
#include "MiniConfig.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class _TBOX_PREFIX_App : public App
{
  public:
    void prepareSettings( Settings* settings )
    {
        readConfig();
        settings->setWindowSize(APP_WIDTH, APP_HEIGHT);
        settings->setMultiTouchEnabled(false);        
    }
    
    void setup()
    {
        auto triMesh = am::triMesh("Teapot");
        auto aabb = triMesh->calcBoundingBox();
        mMesh = gl::VboMesh::create(*triMesh);
        mTexture = am::texture2d("checkerboard");
        
        const vec2 windowSize = toPixels( getWindowSize() );
        mCam = CameraPersp( (int32_t)windowSize.x, (int32_t)windowSize.y, 60.0f, 0.01f, 100.0f );
        mCam.lookAt( aabb.getMax(), aabb.getCenter() );
        mCamUi = CameraUi( &mCam, getWindow(), -1 );
        
        mParams = createConfigUI({200, 200});
    }
    
    void update()
    {
        
    }
    
    void draw()
    {
        gl::setMatrices( mCam );
        gl::clear( Color( 0, 0, 0 ) );
        gl::ScopedTextureBind tex0(mTexture);
        gl::ScopedGlslProg glsl(am::glslProg("texture"));
        gl::draw(mMesh);
    }
    
private:
    gl::VboMeshRef      mMesh;
    gl::Texture2dRef    mTexture;
    CameraPersp         mCam;
    CameraUi            mCamUi;
    
    params::InterfaceGlRef  mParams;
};

CINDER_APP( _TBOX_PREFIX_App, RendererGl )
