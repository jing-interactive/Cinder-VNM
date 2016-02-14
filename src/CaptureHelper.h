#pragma once

#include "cinder/Capture.h"

using namespace ci;
using namespace ci::app;

struct CaptureHelper
{
    Surface         surface;
    gl::TextureRef  texture;
    ivec2           size;
    
    bool isReady()
    {
        return texture != nullptr;
    }
    
    void setup()
    {
        try {
            capture = Capture::create( 640, 480 );
            capture->start();
            size = capture->getSize();
            
            AppBase::get()->getSignalUpdate().connect(std::bind(&CaptureHelper::update, this));
        }
        catch ( ci::Exception &exc ) {
            CI_LOG_EXCEPTION( "Failed to init capture ", exc );
        }
    }
    
    static void printDevices()
    {
        for ( const auto &device : Capture::getDevices() ) {
            console() << "Device: " << device->getName() << " "
#if defined( CINDER_COCOA_TOUCH )
            << ( device->isFrontFacing() ? "Front" : "Rear" ) << "-facing"
#endif
            << std::endl;
        }
    }
    
private:
    
    CaptureRef      capture;

    void update()
    {
        if ( capture && capture->checkNewFrame() ) {
            surface = *capture->getSurface();
            if ( ! texture ) {
                // Capture images come back as top-down, and it's more efficient to keep them that way
                texture = gl::Texture::create( surface, gl::Texture::Format().loadTopDown() );
            }
            else {
                texture->update( surface );
            }
        }
    }
};
