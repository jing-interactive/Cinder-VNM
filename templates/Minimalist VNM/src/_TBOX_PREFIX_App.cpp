#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/CameraUi.h"

#include "AssetManager.h"
#include "MiniConfig.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class _TBOX_PREFIX_App : public App
{
  public:
    void setup() override
    {
        auto aabb = am::triMesh(MESH_NAME)->calcBoundingBox();
        mCam.lookAt(aabb.getMax() * 2.0f, aabb.getCenter());
        mCamUi = CameraUi( &mCam, getWindow(), -1 );
        
        createConfigUI({200, 200});
        gl::enableDepth();
    
        getWindow()->getSignalResize().connect([&] {
            mCam.setAspectRatio( getWindowAspectRatio() );
        });
        
        getWindow()->getSignalDraw().connect([&] {
            gl::setMatrices( mCam );
            gl::clear();
        
            gl::ScopedTextureBind tex0(am::texture2d(TEX0_NAME));
            gl::ScopedGlslProg glsl(am::glslProg(VS_NAME, FS_NAME));
            gl::draw(am::vboMesh(MESH_NAME));
        });
    }
    
private:
    CameraPersp         mCam;
    CameraUi            mCamUi;
};

CINDER_APP( _TBOX_PREFIX_App, RendererGl, [](App::Settings* settings) {
    readConfig();
    settings->setWindowSize(APP_WIDTH, APP_HEIGHT);
    settings->setMultiTouchEnabled(false);
} )
