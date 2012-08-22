
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
AICommand& AICommand::press_haste() { return press(BtnID::HASTE); }
AICommand& AICommand::press_trig1() { return press(BtnID::TRIG_1); }
AICommand& AICommand::press_trig2() { return press(BtnID::TRIG_2); }
AICommand& AICommand::press_wep1() { return press(BtnID::WEP_1); }
AICommand& AICommand::press_wep2() { return press(BtnID::WEP_2); }
AICommand& AICommand::press_wep3() { return press(BtnID::WEP_3); }
AICommand& AICommand::release_haste() { return release(BtnID::HASTE); }
AICommand& AICommand::release_trig1() { return release(BtnID::TRIG_1); }
AICommand& AICommand::release_trig2() { return release(BtnID::TRIG_2); }
AICommand& AICommand::release_wep1() { return release(BtnID::WEP_1); }
AICommand& AICommand::release_wep2() { return release(BtnID::WEP_2); }
AICommand& AICommand::release_wep3() { return release(BtnID::WEP_3); }
AICommand& AICommand::normal_shot(int x, int y) { return move_to(x, y).press_trig1(); }

//deprecate
//AICommand& AICommand::weapon_shot(int x, int y) { return move_to(x, y).press_trig2(); }
