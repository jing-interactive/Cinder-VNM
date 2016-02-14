#pragma once

#include "cinder/MotionManager.h"
#include "cinder/app/App.h"

using namespace ci;
using namespace ci::app;

struct MotionHelper
{
    mat4            deviceRotation;

    void setup()
    {
        CI_LOG_V( "gyro available: " << MotionManager::isGyroAvailable() );
        
        MotionManager::enable( 1000.0f/*, MotionManager::SensorMode::Accelerometer*/ );
        
        AppBase::get()->getSignalUpdate().connect(std::bind(&MotionHelper::update, this));
    }
    
private:
    
    void update()
    {
        if ( MotionManager::isEnabled() )
        {
            deviceRotation = inverse( MotionManager::getRotationMatrix() );
        }
    }
    
};
