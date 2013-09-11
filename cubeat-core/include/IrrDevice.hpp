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

#ifdef _SHOOTING_CUBES_ANDROID_
struct android_app;
#endif

#include <irrlicht.h>
#include <string>

class IrrDevice
{
public:
    struct MatEnumEx {enum{ SCREEN = 0, HARD_LIGHT, VIVID_LIGHT };};

    static IrrDevice& i() {
        static IrrDevice singleton;
        return singleton;
    }

    irr::IrrlichtDevice* d() { return device_; }

    #ifdef _SHOOTING_CUBES_ANDROID_
    bool init( bool test, android_app* app);
    #else
    bool init( bool test );
    #endif
    int  getMaterialTypeBaseCount() const;
    int  getMaterialTypeEx(std::string const& mt_name) const;
    bool run() { return device_->run(); }

    ~IrrDevice() {
        if( device_ ) {
            printf("device releasing....\n");
            device_->drop();
            printf("device releasd.\n");
            device_ = 0;
        }
    }

private:
    IrrDevice():device_(0){}
    IrrDevice(IrrDevice const&);

    irr::IrrlichtDevice* device_;
    static bool inited_;

    static int ADDITIONAL_MATERIAL_BASE_;
};

#endif
