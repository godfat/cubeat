#ifndef _SHOOTING_CUBES_AVI_TEST_
#define _SHOOTING_CUBES_AVI_TEST_

#include "IrrDevice.hpp"
#include "EventDispatcher.hpp"

#ifdef WIN32
#include "private/AVIVideo.hpp"
#endif

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;

using namespace psc;
using namespace ctrl;

struct AVITest
{
public:
    static AVITest& i() {
        static AVITest singleton;
        return singleton;
    }
#ifdef WIN32
    AVITest() : avi("E:/portfolio/ndssp/ndssp_op.avi") {
        IGUIEnvironment* guienv = IrrDevice::i().d()->getGUIEnvironment();
        image = guienv->addImage( avi.getTexture(), position2di(0, 0), false );

        avi.stop();
        EventDispatcher::i().subscribe_timer( bind(&AVIVideo::play, &avi), 3000);
        EventDispatcher::i().subscribe_timer( bind(&AVIVideo::restart, &avi), 5500);
        EventDispatcher::i().subscribe_timer( bind(&AVIVideo::stop, &avi), 7500);
        EventDispatcher::i().subscribe_timer( bind(&AVIVideo::play, &avi), 8500);
        EventDispatcher::i().subscribe_timer( bind(&AVIVideo::setCurrentFrame, &avi, 200), 10000);
    }
#endif

    void cycle() {
#ifdef WIN32
        avi.redraw();
        IrrDevice::i().d()->getGUIEnvironment()->drawAll();
#endif
    }

private:
#ifdef WIN32
    AVIVideo avi;
#endif
    IGUIImage* image;
};

#endif // _SHOOTING_CUBES_AVI_TEST_
