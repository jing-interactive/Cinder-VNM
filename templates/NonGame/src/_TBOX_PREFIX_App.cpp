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
        createConfigUI({200, 200});
    
        getWindow()->getSignalResize().connect([&] {
        });

        getWindow()->getSignalKeyUp().connect([&](KeyEvent& event) {
            if (event.getCode() == KeyEvent::KEY_ESCAPE) quit();
        });
        
        getWindow()->getSignalDraw().connect([&] {
            gl::setMatrices( mCam );
            gl::clear();
        });
    }
    
private:
};

CINDER_APP( _TBOX_PREFIX_App, RendererGl, [](App::Settings* settings) {
    readConfig();
    settings->setWindowSize(APP_WIDTH, APP_HEIGHT);
    settings->setMultiTouchEnabled(false);
} )
