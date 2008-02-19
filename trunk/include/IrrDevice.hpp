#ifndef _SHOOTING_CUBES_IRR_DEVICE_SINGLETON_
#define _SHOOTING_CUBES_IRR_DEVICE_SINGLETON_

/* IrrDevice:
   A Singleton pattern for wrapping up IrrlichtDevice pointer.

   TODO:
   * device can be inited with different params.
   * device can be re-inited.

   * or, don't bother with the init function:
     in the private constructor we can read a setting.ini file,
     and write down all our parameters there (resolution, drivertype,
     fullscreen or not, etc.)
*/

#include <irrlicht.h>

class IrrDevice
{
public:
    static IrrDevice& i() {
        static IrrDevice singleton;
        return singleton;
    }

    irr::IrrlichtDevice* d() { return device_; }

    bool init( bool test );
    bool run() { return device_->run(); }

    ~IrrDevice() {
        printf("device released.\n");
        device_->drop();
    }

private:
    IrrDevice():device_(0){}
    IrrDevice(IrrDevice const&);

    irr::IrrlichtDevice* device_;
    static bool inited_;
};

#endif
