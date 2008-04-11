
/*
   Input Implementation
*/

#include "Input.hpp"
#include "Player.hpp"
#include "view/Scene.hpp"       //warning: is this really good...?
#include "view/Sprite.hpp"      //warning: is this really good...?
#include "IrrDevice.hpp"        //warning: is this really good...?
#include "utils/dictionary.hpp"
#include "private/MastEventReceiver.hpp"

#ifdef _USE_WIIMOTE_
#include "private/Wiimote_IR_internal.hpp"
#endif

#include "Accessors.hpp"

#include <boost/foreach.hpp>
#include <iostream>
#include <string>
#include <fstream>

using namespace irr;

using namespace psc;
using namespace utils;
using namespace ctrl;
using namespace accessor;

//static functions
view::pScene Input::scene_ = view::pScene();
Input::InputList Input::inputs_;
bool Input::keyboard_mouse_input_ = false;

void Input::update_all()
{
    MastEventReceiver::i().endEventProcess();

    //toggle keyboard_mouse_input_ on/off
    if( MastEventReceiver::i().keyPressed( KEY_RETURN ) ) {
        keyboard_mouse_input_ = !keyboard_mouse_input_;
        std::cout<<"Toggle secondary input "<<(keyboard_mouse_input_?"on":"off")<<"\n";
    }

    BOOST_FOREACH( Input* it, inputs_ )
        it->update();

    MastEventReceiver::i().startEventProcess();
}

void Input::init_graphic_items()
{
    scene_ = view::Scene::create("Input scene");
    scene_->setTo2DView();
    BOOST_FOREACH( Input* it, inputs_ )
        it->init_graphic();
}

void Input::redraw_all()
{
    BOOST_FOREACH( Input* it, inputs_ )
        it->redraw();
    scene_->redraw();
}

Input* Input::getInputByIndex(unsigned int i)
{
    if( i >= inputs_.size() ) return 0;
    return inputs_[i];
}

//end of static functions

Input::Input(std::string const& path)
    :cursor_(this), trig1_(this), trig2_(this), wep1_(this), wep2_(this), wep3_(this),
     haste_(this), pause_(this)
{
    map_any keymap = map_any::construct( fetchConfig(path) );
    cursor_key_ = keymap.I("cursor");
    trig1_key_  = keymap.I("trig1");
    trig2_key_  = keymap.I("trig2");
    wep1_key_   = keymap.I("wep1");
    wep2_key_   = keymap.I("wep2");
    wep3_key_   = keymap.I("wep3");
    haste_key_  = keymap.I("haste");
    pause_key_  = keymap.I("pause");

    CURSOR_SENSATIVITY = static_cast<float>( keymap.I("speed") );

#ifdef _USE_WIIMOTE_
    //connect and init wiimote
    wiimote_.Connect();
    wiimote_.SetLEDs(wiimote::TotalConnected());
    wiimote_.SetReportType(wiimote::IN_BUTTONS_ACCEL_IR);
    //test
    std::cout << "Wiimote connected: " << wiimote::TotalConnected() << "\n";
#endif
    inputs_.push_back( this );

    cursor_texture_name_ = keymap.S("cursor_texture");
    area_texture_name_ = keymap.S("area_rect");
}

void Input::update()
{
    write_state_now_to_last();

    if( keyboard_mouse_input_ ) {
        cursor_by_keyboard_mouse();
        buttons_by_keyboard_mouse();
    }

#ifdef _USE_WIIMOTE_
    float center_x, center_y, zdepth;
    wiimote_.RefreshState();

    if( interpret_ir_data(wiimote_, center_x, center_y, zdepth) ) {
        cursor_.x() = static_cast<int>(center_x * cursor_.width());
        cursor_.y() = static_cast<int>(center_y * cursor_.height());
    }
    trig1_.now() |= wiimote_.Button.A();
    trig2_.now() |= wiimote_.Button.B();
    wep1_.now() |= wiimote_.Button.Left();
    wep2_.now() |= wiimote_.Button.Right();
    wep3_.now() |= wiimote_.Button.Up();
    haste_.now() |= wiimote_.Button.Down();
    pause_.now() |= wiimote_.Button.Home();
#endif

    update_btn_state();
    cursor_.constrain();
}

void Input::init_graphic()
{
    cursor_mark_ = view::Sprite::create(cursor_texture_name_, scene_, 128, 128, true);
    range_shape_ = view::Sprite::create(area_texture_name_, scene_, 191, 191, true);
    //must use config or changes dynamically when weapon's area predicate changes.
    range_shape_->set<Alpha>(192);
    range_shape_->set<Visible>(false);
}

void Input::redraw()
{
    cursor_mark_->moveTo(cursor_.x(), cursor_.y());
    range_shape_->moveTo(cursor_.x(), cursor_.y());
}

void Input::setRangeShapeVisible(bool b) {
    if( range_shape_ )
        range_shape_->set<Visible>(b);
}

void Input::setCursorVisible(bool b) {
    if( cursor_mark_ )
        cursor_mark_->set<Visible>(b);
}

void Input::cursor_by_keyboard_mouse()
{
    float speed = CURSOR_SENSATIVITY * 150.f / IrrDevice::i().d()->getVideoDriver()->getFPS();
    if( speed < 1.f ) speed = 1.f;
    if( cursor_key_ == 0 ) {     //A Special Case Which Is Not EKEY_CODE: 0x00 means "mouse"
        cursor_.x() = MastEventReceiver::i().mouseX();
        cursor_.y() = MastEventReceiver::i().mouseY();
    }
    else if( cursor_key_ == 1 ) {//A Special Case Which Is Not EKEY_CODE: 0x01 means "wasd"
        cursor_.x() += (static_cast<int>(MastEventReceiver::i().keyDown(KEY_KEY_D)) -
                        static_cast<int>(MastEventReceiver::i().keyDown(KEY_KEY_A))) * speed;
        cursor_.y() += (static_cast<int>(MastEventReceiver::i().keyDown(KEY_KEY_S)) -
                        static_cast<int>(MastEventReceiver::i().keyDown(KEY_KEY_W))) * speed;
    }
    else if( cursor_key_ == 2 ) {//A Special Case Which Is Not EKEY_CODE: 0x02 means "use arrow"
        cursor_.x() += (static_cast<int>(MastEventReceiver::i().keyDown(KEY_RIGHT)) -
                        static_cast<int>(MastEventReceiver::i().keyDown(KEY_LEFT ))) * speed;
        cursor_.y() += (static_cast<int>(MastEventReceiver::i().keyDown(KEY_DOWN )) -
                        static_cast<int>(MastEventReceiver::i().keyDown(KEY_UP   ))) * speed;
    }
}

void Input::buttons_by_keyboard_mouse()
{
    trig1_.now() = MastEventReceiver::i().keyDown( trig1_key_ );
    trig2_.now() = MastEventReceiver::i().keyDown( trig2_key_ );
    wep1_.now() = MastEventReceiver::i().keyDown( wep1_key_ );
    wep2_.now() = MastEventReceiver::i().keyDown( wep2_key_ );
    wep3_.now() = MastEventReceiver::i().keyDown( wep3_key_ );
    haste_.now() = MastEventReceiver::i().keyDown( haste_key_ );
    pause_.now() = MastEventReceiver::i().keyDown( pause_key_ );
}

void Input::write_state_now_to_last()
{
    cursor_.write_state_now_to_last();
    trig1_.write_state_now_to_last();
    trig2_.write_state_now_to_last();
    wep1_.write_state_now_to_last();
    wep2_.write_state_now_to_last();
    wep3_.write_state_now_to_last();
    haste_.write_state_now_to_last();
    pause_.write_state_now_to_last();
}

void Input::update_btn_state()
{
    trig1_.update_state();
    trig2_.update_state();
    wep1_.update_state();
    wep2_.update_state();
    wep3_.update_state();
    haste_.update_state();
    pause_.update_state();
}

Input&  Input::player(wpPlayer player) { player_ = player; return *this; }
pPlayer Input::player() const { return player_.lock(); }

Input::~Input()
{
    //
}
