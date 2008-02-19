
#ifndef _SHOOTING_CUBES_CONFIGURE_CONSTANTS_
#define _SHOOTING_CUBES_CONFIGURE_CONSTANTS_

/* a program-wide config class */

#include "utils/dictionary.hpp"
#include <string>

namespace psc {

struct Conf
{
private:
    utils::map_any config;
    Conf() :
        config( utils::map_any::construct(utils::fetchConfig("config.zzml")) ),
        CONFIG_PATH      ( config.S("config_path") ),
        CONFIG_INPUT_PATH( config.S("input_conf_path") ),
        CONFIG_PRESENTER_PATH( config.S("presenter_conf_path") ),
        SCREEN_W         ( config.I("screenwidth") ),
        SCREEN_H         ( config.I("screenheight") ),
        FRAMERATE        ( config.I("framerate") )
    {
    }

public:
    static Conf& i() {
        static Conf singleton;
        return singleton;
    }

    std::string const CONFIG_PATH;
    std::string const CONFIG_INPUT_PATH;
    std::string const CONFIG_PRESENTER_PATH;
    int const SCREEN_W;
    int const SCREEN_H;
    int const FRAMERATE;
};

}

#endif // _SHOOTING_CUBES_CONFIGURE_CONSTANTS_
