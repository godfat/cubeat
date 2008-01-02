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
    static bool init(bool test) {         //param init test.
        if( device_ != 0 ) return true;
        static IrrDevice singleton(test);
        if( singleton.device_ == 0 ) return false;
        return true;
    }

    static irr::IrrlichtDevice* i() {
        if( device_ != 0 ) return device_;
        static IrrDevice singleton(false);
        return singleton.device_;
    }

    ~IrrDevice() {
        printf("device released.\n");
        device_->drop();
    }

private:
    IrrDevice( bool test );
    IrrDevice(IrrDevice const&);

    static irr::IrrlichtDevice* device_;
};

#endif
