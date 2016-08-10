#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/CameraUi.h"
#include "cinder/Log.h"

#include "AssetManager.h"
#include "MiniConfig.h"
#include "DepthSensor.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class _TBOX_PREFIX_App : public App
{
  public:
    void setup() override
    {
        const auto& args = getCommandLineArgs();
        log::makeLogger<log::LoggerFile>();

        Kinect::DeviceType type = (Kinect::DeviceType)SENSOR_TYPE;
        Kinect::Device::Option option;
        option.enableColor = true;
        mDevice = Kinect::Device::create(type, option);
        if (!mDevice->isValid())
        {
            quit();
            return;
        }

        mDevice->signalDepthDirty.connect([&]{
            //
        });

        mCamUi = CameraUi( &mCam, getWindow(), -1 );
        
        createConfigUI({200, 200});
        gl::enableDepth();
    
        getWindow()->getSignalResize().connect([&] {
            mCam.setAspectRatio( getWindowAspectRatio() );
        });

        getWindow()->getSignalKeyUp().connect([&](KeyEvent& event) {
            if (event.getCode() == KeyEvent::KEY_ESCAPE) quit();
        });
        
        //mGlslProg = am::glslProg(VS_NAME, FS_NAME);
        //mGlslProg->uniform("uTex0", 0);

        getWindow()->getSignalDraw().connect([&] {
            gl::setMatrices( mCam );
            gl::clear();
        });
    }
    
private:
    CameraPersp         mCam;
    CameraUi            mCamUi;
    gl::GlslProgRef     mGlslProg;
    Kinect::DeviceRef   mDevice;
};

CINDER_APP( _TBOX_PREFIX_App, RendererGl, [](App::Settings* settings) {
    readConfig();
    settings->setWindowSize(APP_WIDTH, APP_HEIGHT);
    settings->setMultiTouchEnabled(false);
} )
