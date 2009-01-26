
//A Singleton pattern for wrapping up IrrlichtDevice pointer.
//See "IrrDevice.hpp"

#include "IrrDevice.hpp"
#include "private/MastEventReceiver.hpp"
#include "Conf.hpp"
#include "utils/dictionary.hpp"

using namespace irr;

using namespace psc;
using namespace utils;

bool IrrDevice::inited_ = false;

bool IrrDevice::init(bool test)
{
    map_any config = map_any::construct( fetchConfig( Conf::i().CONFIG_PATH +
                                                      "device.zzml") );
    SIrrlichtCreationParameters param;
    param.AntiAlias        = false;
    param.Bits             = config.I("bits");
    param.DriverType       = video::EDT_OPENGL;
    param.EventReceiver    = &MastEventReceiver::i();
    param.Fullscreen       = static_cast<bool>(config.I("fullscreen"));
    param.HighPrecisionFPU = true;
    param.Stencilbuffer    = false;
    param.Vsync            = static_cast<bool>(config.I("vsync"));
    param.WindowSize       = core::dimension2di(Conf::i().SCREEN_W, Conf::i().SCREEN_H);

    if( inited_ ) return false;
    inited_ = true;

    if( !test )
        device_ = createDevice( video::EDT_NULL );
    else
        device_ = createDeviceEx(param);

    //device_->getLogger()->setLogLevel(ELL_NONE);

    return device_ ? true : false;
}
