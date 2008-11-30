#ifndef _SHOOTING_CUBES_CTRL_BUTTON_ENUM_
#define _SHOOTING_CUBES_CTRL_BUTTON_ENUM_

namespace psc{ namespace ctrl {
enum BSTATE{
    BTN_UP = 0,
    BTN_DOWN = 1,
    BTN_PRESS = 2,
    BTN_RELEASE = 3
};
enum FSTATE {
    FOCUS_ON,
    FOCUS_OFF,
    FOCUS_ENTER,
    FOCUS_LEAVE
}; //Focus state
}
}

#endif // _SHOOTING_CUBES_CTRL_BUTTON_ENUM_
