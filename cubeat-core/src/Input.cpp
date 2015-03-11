
/*
   Input Implementation
*/

#include "Input.hpp"
#include "Player.hpp"
#include "view/Scene.hpp"       //warning: is this really good...?
#include "view/Sprite.hpp"      //warning: is this really good...?
#include "IrrDevice.hpp"        //warning: is this really good...?
#include "utils/Logger.hpp"
#include "utils/dictionary.hpp"
#include "utils/Replay.hpp"
#include "Conf.hpp"
#include "App.hpp"
#include "private/MastEventReceiver.hpp"

#ifdef _USE_WIIMOTE_
// 2011.04.18: code removed due to possibly unresolved copyright issue.
// if you define _USE_WIIMOTE_, this code simply cannot compile.
#include "private/Wiimote_IR_internal.hpp"
#include "EventDispatcher.hpp"
#include "ctrl/TimerDispatcher.hpp"
#endif

#include "private/MouseState.hpp"

#include "Accessors.hpp"
#include "EasingEquations.hpp"

#include <boost/foreach.hpp>
#include <iostream>
#include <string>

using namespace irr;

using namespace psc;
using namespace utils;
using namespace ctrl;
using namespace easing;
using namespace accessor;

InputMgr::InputMgr()
    :MAX_INPUTS(2), keyboard_mouse_input_(false), inited_(false),
     window_focus_now_(false), window_focus_last_(false), mice_detected_by_manymouse_(0),
     poll_manymouse_event_(0)
{
    std::cout << "InputMgr constructed." << std::endl;
}

InputMgr::~InputMgr()
{
    std::cout << "InputMgr destructed." << std::endl;
    cleanupInputs();
}

bool InputMgr::createInputs()
{
    if( inited_ ) return false;
    inited_ = true;

    initManyMouse();

    std::cout << "InputMgr created inputs." << std::endl;
    for( int i = 0; i < MAX_INPUTS; ++i )
        inputs_.push_back( new Input( find_input_name_accordingly(i) ) );

    initGraphicItems();
    return true;
}

std::string InputMgr::find_input_name_accordingly(int const& index)
{
    std::string name("input/player");
    name += (index + 1 + 48); //ascii-code...
    if( index > 0 && index >= manyMouseCount() )
        name += "_keyboard";  //mouse not enough for this player
    else name += "_mouse";    //mouse enough for this player
    name += ".zzml";
    return name;
}

bool InputMgr::cleanupInputs()
{
    if( !inited_ ) return false;
    std::cout << "InputMgr cleared inputs." << std::endl;
    BOOST_FOREACH(Input* it, inputs_) {
        if( it != 0 )
            delete it;
    }

    quitManyMouse();
    IrrDevice::i().d()->getCursorControl()->setVisible(true);

    inited_ = false;
    return true;
}

void InputMgr::initManyMouse()
{
    using std::tr1::bind;
#if !defined(_SHOOTING_CUBES_ANDROID_) && !defined(_SHOOTING_CUBES_IPHONE_)
    if( int mice_detected_initially = ManyMouse_Init() ) {
#else
    if( int mice_detected_initially = 0 ) {
#endif
        std::cout << "ManyMouse_Init: " << mice_detected_initially << std::endl;
        if( !poll_manymouse_event_ )
            poll_manymouse_event_ = bind(&InputMgr::pollManyMouseStates, this);
    }
    else std::cout << "ManyMouse_Init failed." << std::endl;
}

void InputMgr::quitManyMouse()
{
    poll_manymouse_event_ = 0;
#if !defined(_SHOOTING_CUBES_ANDROID_) && !defined(_SHOOTING_CUBES_IPHONE_)
    ManyMouse_Quit();
    std::cout << "ManyMouse_Quit." << std::endl;
#endif
}

void InputMgr::pollManyMouseStates()
{
#if !defined(_SHOOTING_CUBES_ANDROID_) && !defined(_SHOOTING_CUBES_IPHONE_)
    ManyMouseEvent e;
    while ( ManyMouse_PollEvent(&e) )
    {
        pMouseState ms;
        for( int i = 0; i < MAX_INPUTS; ++i ) {
            if( inputs_[i]->state_->device_id == e.device ) {
                ms = inputs_[i]->state_;
                break;
            }
        }
        if( !ms ) {
            if( mice_detected_by_manymouse_ < MAX_INPUTS )
                associate_input_manymouse(e.device);
            continue;
        }

        switch ( e.type ) {
            case MANYMOUSE_EVENT_RELMOTION: {
                if (e.item == 0)      ms->x += e.value;
                else if (e.item == 1) ms->y += e.value;
                break;
            }
            case MANYMOUSE_EVENT_ABSMOTION: {
                float val =    static_cast<float>(e.value - e.minval);
                float maxval = static_cast<float>(e.maxval - e.minval);
                if (e.item == 0)      ms->x = (val / maxval) * Conf::i().SCREEN_W();
                else if (e.item == 1) ms->y = (val / maxval) * Conf::i().SCREEN_H();
                break;
            }
            case MANYMOUSE_EVENT_BUTTON: {
                if (e.item < 32) {
                    if (e.value) ms->buttons |= (1 << e.item);
                    else         ms->buttons &= ~(1 << e.item);
                }
                break;
            }
            case MANYMOUSE_EVENT_DISCONNECT: {
                ms->connected = false;
                handleManyMouseDisconnect();
                break;
            }
            default: break;
        }
    }
#endif
}

void InputMgr::associate_input_manymouse(unsigned int const& device_id)
{
#if !defined(_SHOOTING_CUBES_ANDROID_) && !defined(_SHOOTING_CUBES_IPHONE_)
    ++mice_detected_by_manymouse_; //add first.

    Input* it = inputs_[mice_detected_by_manymouse_ - 1];
    pMouseState ms = it->state_;
    ms->name = ManyMouse_DeviceName(device_id);
    ms->device_id = device_id;
    ms->connected = true;

    it->reinit_config( find_input_name_accordingly(mice_detected_by_manymouse_ - 1) );
    std::cout << "Mice #" << mice_detected_by_manymouse_ - 1 << ": " << ms->name << std::endl;
#endif
}

void InputMgr::handleManyMouseDisconnect()
{
}

void InputMgr::reinitManyMouse()
{
    quitManyMouse();
    initManyMouse();
}

bool InputMgr::keyDown(unsigned char const& code) const {
    return MastEventReceiver::i().keyDown( code );
}

bool InputMgr::keyPressed(unsigned char const& code) const {
    return MastEventReceiver::i().keyPressed( code );
}

bool InputMgr::keyReleased(unsigned char const& code) const {
    return MastEventReceiver::i().keyReleased( code );
}

void InputMgr::updateAll()
{
    if( poll_manymouse_event_ )
        poll_manymouse_event_();

    //MastEventReceiver::i().endEventProcess();

    //toggle keyboard_mouse_input_ on/off
    if( window_focus_now_ ) {
        if( keyboard_mouse_input_ && MastEventReceiver::i().keyPressed( KEY_ESCAPE ) ) {
            toggleInput(false);
        }
        else if( !keyboard_mouse_input_ && MastEventReceiver::i().leftMousePressed() ) {
            toggleInput(true);
        }

        if( MastEventReceiver::i().keyDown( KEY_LSHIFT ) && MastEventReceiver::i().keyPressed( KEY_KEY_Z ) ) {
            App::i().cheat1();
        }
    }

    window_focus_last_ = window_focus_now_;
    window_focus_now_ = IrrDevice::i().d()->isWindowActive();

    if( windowGotFocus() )           toggleInput(true);
    else if( windowReleasedFocus() ) toggleInput(false);

    if( mice_detected_by_manymouse_ > 0 && keyboard_mouse_input_ )
        IrrDevice::i().d()->getCursorControl()->setPosition(0.5f, 0.5f); //grab system cursor

    BOOST_FOREACH( Input* it, inputs_ ) {
        it->update();
    }

    //MastEventReceiver::i().startEventProcess();
}

void InputMgr::toggleInput(bool const& flag)
{
    if( keyboard_mouse_input_ == flag ) return;
    keyboard_mouse_input_ = flag;
    std::cout << "Toggle input grabbing: " << (keyboard_mouse_input_?"on":"off") << std::endl;
    if( keyboard_mouse_input_ ) {
        reinitManyMouse();
        IrrDevice::i().d()->getCursorControl()->setVisible(false);
    }
    else {
        IrrDevice::i().d()->getCursorControl()->setVisible(true);
    }
}

void InputMgr::initGraphicItems()
{
    if( !scene_ ) {
        scene_ = view::Scene::create("input");
        scene_->setTo2DView();
    }
    BOOST_FOREACH( Input* it, inputs_ )
        it->init_graphic();
}

void InputMgr::redrawAll()
{
    BOOST_FOREACH( Input* it, inputs_ )
        it->redraw();
    scene_->redraw();
}

Input* InputMgr::getInputByIndex(unsigned int i)
{
    if( i >= inputs_.size() ) return 0;
    return inputs_[i];
}

/// InputManager above, Input below ///

Input::Input(std::string const& path)
    :cursor_(this), trig1_(this), trig2_(this), wep1_(this), wep2_(this), wep3_(this),
     haste_(this), pause_(this), ai_controlled_(false), update_cursor_event_(0),
     update_buttons_event_(0)
{
    state_ = MouseState::create();
    state_->name = "manymouse state not initialized.";
    state_->device_id = 0xffffffff; // since its unsigned int, we have to use other garbage value.
    state_->connected = false;
#ifdef _USE_WIIMOTE_
    wiimote_.Connect();
    wiimote_.SetLEDs(wiimote::TotalConnected());
    wiimote_.SetReportType(wiimote::IN_BUTTONS_ACCEL_IR);
    std::cout << "Wiimote connected: " << wiimote::TotalConnected() << std::endl;
#endif //_USE_WIIMOTE_

    reinit_config(path);
}

void Input::reinit_config(std::string const& path)
{
    map_any keymap = Conf::i().config_of(path);
    cursor_key_ = keymap.I("cursor");
    trig1_key_  = keymap.I("trig1");
    trig2_key_  = keymap.I("trig2");
    wep1_key_   = keymap.I("wep1");
    wep2_key_   = keymap.I("wep2");
    wep3_key_   = keymap.I("wep3");
    haste_key_  = keymap.I("haste");
    pause_key_  = keymap.I("pause");
    cursor_texture_name_ = keymap.S("cursor_texture");
    area_texture_name_ = keymap.S("area_rect");

    cursor_sensitivity_ = static_cast<float>( keymap.I("speed") );

    using std::tr1::bind;
    switch( cursor_key_ ) {
        case 0:
            update_cursor_event_ = state_->connected ?
                bind(&Input::update_cursor_by_manymouse, this) :
                bind(&Input::update_cursor_by_sysmouse, this);
            break;
        case 1:
            update_cursor_event_ = bind(&Input::update_cursor_by_wasd, this);
            break;
        case 2:
            update_cursor_event_ = bind(&Input::update_cursor_by_arrowkeys, this);
            break;
    }

    update_buttons_event_ = state_->connected ?
        bind(&Input::update_buttons_by_manymouse, this) :
        bind(&Input::update_buttons_by_sysmouse_or_keyboard, this);
}

void Input::update()
{
    if( ai_controlled_ ) { //AI integration testing
        //This will update AI's button state using AI's simulated input of LAST FRAME.
        App::i().getReplay().set_input_for(this);
        update_btn_state(); //that's why we have to call update_btn_state() first when it's ai_controlled_.
        write_state_now_to_last();
        if( InputMgr::i().keyboardMouseInput() ) {
            buttons_by_keyboard_mouse(); //but in case you still want to have some control over user input
            //we'll just use data from polled device data as well. AI's simulated input will overwrite
            //some of them THIS FRAME.
        }
    }
    else {
        write_state_now_to_last();
        if( InputMgr::i().keyboardMouseInput() ) {
            buttons_by_keyboard_mouse();
            cursor_by_keyboard_mouse();
        }
#ifdef _USE_WIIMOTE_
        update_by_wiimote();
#endif // _USE_WIIMOTE_
        update_btn_state();
    }
    cursor_.constrain();
    state_->constrain();
}

#ifdef _USE_WIIMOTE_
void Input::update_by_wiimote()
{
    float center_x, center_y, zdepth;
    wiimote_.RefreshState();

    if( interpret_ir_data(wiimote_, center_x, center_y, zdepth) ) {
//        int new_x = static_cast<int>(center_x * cursor_.width() * 1.2f);
//        int new_y = static_cast<int>(center_y * cursor_.height() * 1.2f);
        int new_x = static_cast<int>(wiimote_.IR.Dot[0].X * cursor_.width());
        int new_y = static_cast<int>(wiimote_.IR.Dot[0].Y * cursor_.height());
        cursor_.x() = new_x;
        cursor_.y() = new_y;
        cursor_.x() = (cursor_.x() + cursor_.lx()) / 2;
        cursor_.y() = (cursor_.y() + cursor_.ly()) / 2;
    }
    trig1_.now() |= wiimote_.Button.A();
    trig2_.now() |= wiimote_.Button.B();
    wep1_.now() |= wiimote_.Button.Left();
    wep2_.now() |= wiimote_.Button.Right();
    wep3_.now() |= wiimote_.Button.Up();
    haste_.now() |= wiimote_.Button.Down();
    pause_.now() |= wiimote_.Button.Home();
}

void Input::rumbleWiimote(int ms)
{
    using std::tr1::bind;
    wiimote_.SetRumble(true);
    EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
        bind(&wiimote::SetRumble, &wiimote_, false), ms );
}
#endif //_USE_WIIMOTE_

void Input::init_graphic()
{
    cursor_mark_ = view::Sprite::create(cursor_texture_name_, InputMgr::i().scene(), 64, 64, true);
    cursor_mark_->tween<Linear, Rotation>(vec3(0,0,-360), 3000u, -1);
    range_shape_ = view::Sprite::create(area_texture_name_, InputMgr::i().scene(), 191, 191, true);
    //must use config or changes dynamically when weapon's area predicate changes.
    range_shape_->set<Alpha>(192);
    range_shape_->set<Visible>(false);
}

void Input::redraw()
{
    if( !ai_controlled_ ) {
        cursor_mark_->moveTo(cursor_.x(), cursor_.y());
        range_shape_->moveTo(cursor_.x(), cursor_.y());
    }
    if( trig1_.pressed() ) //note: temporary effects //2011.03.28 right-click effect removed.
        cursor_mark_->tween<OBack, Scale>(vec3(.7,.7,.7), vec3(1,1,1), 300u);
}

view::pSprite Input::getCursor() const
{
    return cursor_mark_;
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
    update_cursor_event_();
}

void Input::update_cursor_by_manymouse()
{
    cursor_.x() = state_->x;
    cursor_.y() = state_->y;
}

void Input::update_cursor_by_sysmouse()
{
    cursor_.x() = MastEventReceiver::i().mouseX();
    cursor_.y() = MastEventReceiver::i().mouseY();
}

void Input::update_cursor_by_wasd()
{
    float speed = cursor_sensitivity_ * 150.f / IrrDevice::i().d()->getVideoDriver()->getFPS();
    if( speed < 1.f ) speed = 1.f;
    cursor_.x() += (static_cast<int>(MastEventReceiver::i().keyDown(KEY_KEY_D)) -
                    static_cast<int>(MastEventReceiver::i().keyDown(KEY_KEY_A))) * speed;
    cursor_.y() += (static_cast<int>(MastEventReceiver::i().keyDown(KEY_KEY_S)) -
                    static_cast<int>(MastEventReceiver::i().keyDown(KEY_KEY_W))) * speed;
}

void Input::update_cursor_by_arrowkeys()
{
    float speed = cursor_sensitivity_ * 150.f / IrrDevice::i().d()->getVideoDriver()->getFPS();
    if( speed < 1.f ) speed = 1.f;
    cursor_.x() += (static_cast<int>(MastEventReceiver::i().keyDown(KEY_RIGHT)) -
                    static_cast<int>(MastEventReceiver::i().keyDown(KEY_LEFT ))) * speed;
    cursor_.y() += (static_cast<int>(MastEventReceiver::i().keyDown(KEY_DOWN )) -
                    static_cast<int>(MastEventReceiver::i().keyDown(KEY_UP   ))) * speed;
}

void Input::buttons_by_keyboard_mouse()
{
    //the implementation here now raises a perculier problem. please see
    //2011_03_25_wide_adjust_branch.txt for details.
    update_buttons_event_();
    wep1_.now() = MastEventReceiver::i().keyDown( wep1_key_ );
    wep2_.now() = MastEventReceiver::i().keyDown( wep2_key_ );
    wep3_.now() = MastEventReceiver::i().keyDown( wep3_key_ );
    //pause_.now() = MastEventReceiver::i().keyDown( pause_key_ );
    haste_.now() = MastEventReceiver::i().keyDown( haste_key_ );
}

void Input::update_buttons_by_manymouse()
{
#if !defined(_SHOOTING_CUBES_ANDROID_) && !defined(_SHOOTING_CUBES_IPHONE_)
    unsigned int button_bits = state_->buttons;
    trig1_.now() = (button_bits & trig1_key_) > 0;
    trig2_.now() = (button_bits & trig2_key_) > 0;
    //haste_.now() = (button_bits & haste_key_) > 0;
    pause_.now() = (button_bits & pause_key_) > 0;
#endif
}

void Input::update_buttons_by_sysmouse_or_keyboard()
{
    trig1_.now() = MastEventReceiver::i().keyDown( trig1_key_ );
    trig2_.now() = MastEventReceiver::i().keyDown( trig2_key_ );
    //haste_.now() = MastEventReceiver::i().keyDown( haste_key_ );
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

    App::i().getReplay().record_input_state(this);
}


Input&  Input::player(wpPlayer player) { player_ = player; return *this; }
pPlayer Input::player() const { return player_.lock(); }

Input::~Input()
{
    std::cout << "Input destructing ..." << std::endl;
}
