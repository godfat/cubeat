
#include "model/AICommand.hpp"

using namespace psc;
using namespace model;

AICommand& AICommand::move_to(int x, int y)
{
    pos_ = pPosition( new std::pair<int, int>(x, y) );
    return *this;
}

AICommand& AICommand::press(BtnID::BID btn)
{
    btn_ = pButton( new BtnID::BID( btn ) );
    btn_press_ = true;
    return *this;
}

AICommand& AICommand::release(BtnID::BID btn)
{
    btn_ = pButton( new BtnID::BID( btn ) );
    btn_press_ = false;
    return *this;
}

//shortcuts
AICommand& AICommand::press_haste() { press(BtnID::HASTE); return *this; }
AICommand& AICommand::press_trig1() { press(BtnID::TRIG_1); return *this; }
AICommand& AICommand::press_trig2() { press(BtnID::TRIG_2); return *this; }
AICommand& AICommand::press_wep1() { press(BtnID::WEP_1); return *this; }
AICommand& AICommand::press_wep2() { press(BtnID::WEP_2); return *this; }
AICommand& AICommand::press_wep3() { press(BtnID::WEP_3); return *this; }
AICommand& AICommand::release_haste() { release(BtnID::HASTE); return *this; }
AICommand& AICommand::release_trig1() { release(BtnID::TRIG_1); return *this; }
AICommand& AICommand::release_trig2() { release(BtnID::TRIG_2); return *this; }
AICommand& AICommand::release_wep1() { release(BtnID::WEP_1); return *this; }
AICommand& AICommand::release_wep2() { release(BtnID::WEP_2); return *this; }
AICommand& AICommand::release_wep3() { release(BtnID::WEP_3); return *this; }
AICommand& AICommand::normal_shot(int x, int y) { move_to(x, y).press_trig1(); return *this; }
AICommand& AICommand::weapon_shot(int x, int y) { move_to(x, y).press_trig2(); return *this; }
