
#ifndef _SHOOTING_CUBES_DETAIL_MT_APP_RUN_
#define _SHOOTING_CUBES_DETAIL_MT_APP_RUN_

#include "EventDispatcher.hpp"
#include "Input.hpp"
#include "IrrDevice.hpp"

#include "presenter/Object.hpp"
#include "presenter/Transitioner.hpp"

namespace psc{

template <class Tester>
int App::run(Tester tester)
{
    using namespace irr;
    using namespace core;
    using namespace scene;
    using namespace video;

    using namespace ctrl;

    IVideoDriver* driver = IrrDevice::i().d()->getVideoDriver();
    int lastFPS = -1;

    while( IrrDevice::i().run() ) {
        if( IrrDevice::i().d()->isWindowActive() /*&& !update_block()*/ )
        {
            if( IrrDevice::i().d()->getTimer()->isStopped() )
                IrrDevice::i().d()->getTimer()->start();
            Input::update_all();
            EventDispatcher::i().dispatch();

            driver->beginScene(true, true, video::SColor(0,0,0,0));
            master_presenter_->cycle();
            driver->clearZBuffer();  //clear z-buffer to overlay the whole scene
            trans_->cycle();
            driver->clearZBuffer();

            if(tester) (*tester)();

            driver->endScene();

            //FPS for debug
            int fps = driver->getFPS();
            if( fps != lastFPS ) {
                core::stringw str(L"FPS: ");
                str += fps;
                IrrDevice::i().d()->setWindowCaption( str.c_str() );
                lastFPS = fps;
            }
            if( temp_presenter_ ) { //hand over master presenter here "safely."
                master_presenter_ = temp_presenter_;
                temp_presenter_.reset();
            }
        }
        else
            if( !IrrDevice::i().d()->getTimer()->isStopped() )
                IrrDevice::i().d()->getTimer()->stop();
    }

    return 0;
}

} // end of namespace psc

#endif
