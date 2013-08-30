
//A Singleton pattern for wrapping up IrrlichtDevice pointer.
//See "IrrDevice.hpp"

#include "IrrDevice.hpp"
#include "private/MastEventReceiver.hpp"
#include "Conf.hpp"
#include "utils/dictionary.hpp"

#include "utils/Logger.hpp"

using namespace irr;

using namespace psc;
using namespace utils;

bool IrrDevice::inited_ = false;
int  IrrDevice::ADDITIONAL_MATERIAL_BASE_ = video::EMT_TRANSPARENT_MODULATE;

class ShaderCallBack : public irr::video::IShaderConstantSetCallBack
{
public:

	virtual void OnSetConstants(video::IMaterialRendererServices* services,
			s32 userData)
	{
        int d[2] = {0, 1};
        // WTF: I think I'll never understand why passing two simple integer
        //      has to use reinterpret_cast here.
        // These two line are just simply notify pixel shader that we have two
        // uniform sampler2D, which are texture numbers.

        /// WTF MEMO: These are declared deprecated in Irrlicht 1.9
        ///           use getPixelShaderConstantID by name first and pass the id.
        services->setPixelShaderConstant("selfTexture", (float*)&d[0], 1);
        services->setPixelShaderConstant("bgBuffer", (float*)&d[1], 1);
	}
};

#ifdef _SHOOTING_CUBES_ANDROID_
bool IrrDevice::init(bool test, android_app* app)
#else
bool IrrDevice::init(bool test)
#endif
{
    map_any config = Conf::i().config_of("device");
    SIrrlichtCreationParameters param;
    param.AntiAlias        = false;

    param.EventReceiver    = &MastEventReceiver::i();
    param.HighPrecisionFPU = true;
    param.Stencilbuffer    = false;
    param.Vsync            = static_cast<bool>(config.I("vsync"));
    param.WindowSize       = core::dimension2di(Conf::i().SCREEN_W(), Conf::i().SCREEN_H());
    #ifdef _SHOOTING_CUBES_ANDROID_
    param.Bits             = 24;
    param.ZBufferBits      = 16;
    param.DriverType       = video::EDT_OGLES2;
    param.PrivateData      = app;
    #else
    param.Bits             = config.I("bits");
    param.DriverType       = video::EDT_OPENGL;
    param.Fullscreen       = static_cast<bool>(config.I("fullscreen"));
    #endif

    if( inited_ ) return false;
    inited_ = true;

    if( !test )
        device_ = createDevice( video::EDT_NULL );
    else
        device_ = createDeviceEx(param);

    //device_->getLogger()->setLogLevel(ELL_NONE);

    // 2012.10.1 Since this is a one-time graphic driver change, I think it's best these
    // are being added here.
    video::IVideoDriver* driver = device_->getVideoDriver();
    video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();
    ShaderCallBack* cb = new ShaderCallBack();

    // WTF: Why pixelMain? there's no pixelMain.... but it runs.
#if defined(_SHOOTING_CUBES_ANDROID_)
#else
    ADDITIONAL_MATERIAL_BASE_ =
        gpu->addHighLevelShaderMaterialFromFiles("", "", video::EVST_VS_1_1, Conf::i().shader_path("screen.frag").c_str(), "pixelMain", video::EPST_PS_1_1, cb, video::EMT_TRANSPARENT_MODULATE);
    gpu->addHighLevelShaderMaterialFromFiles("", "", video::EVST_VS_1_1, Conf::i().shader_path("hardlight.frag").c_str(), "pixelMain", video::EPST_PS_1_1, cb, video::EMT_TRANSPARENT_MODULATE);
    gpu->addHighLevelShaderMaterialFromFiles("", "", video::EVST_VS_1_1, Conf::i().shader_path("vividlight.frag").c_str(), "pixelMain", video::EPST_PS_1_1, cb, video::EMT_TRANSPARENT_MODULATE);
#endif
    cb->drop();

    return device_ ? true : false;
}

int IrrDevice::getMaterialTypeBaseCount() const
{
    return ADDITIONAL_MATERIAL_BASE_;
}

int IrrDevice::getMaterialTypeEx(std::string const& mt_name) const
{
    if( mt_name == "SCREEN" )      return ADDITIONAL_MATERIAL_BASE_ + static_cast<int>(MatEnumEx::SCREEN);
    if( mt_name == "HARD_LIGHT" )  return ADDITIONAL_MATERIAL_BASE_ + static_cast<int>(MatEnumEx::HARD_LIGHT);
    if( mt_name == "VIVID_LIGHT" ) return ADDITIONAL_MATERIAL_BASE_ + static_cast<int>(MatEnumEx::VIVID_LIGHT);
    return video::EMT_TRANSPARENT_MODULATE;
}
