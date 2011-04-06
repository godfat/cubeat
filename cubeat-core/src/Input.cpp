
/*
   Input Implementation
*/

#include "Input.hpp"
#include "EventDispatcher.hpp"
#include "Player.hpp"
#include "view/Scene.hpp"       //warning: is this really good...?
#include "view/Sprite.hpp"      //warning: is this really good...?
#include "IrrDevice.hpp"        //warning: is this really good...?
#include "utils/dictionary.hpp"
#include "Conf.hpp"
#include "private/MastEventReceiver.hpp"

#ifdef _USE_WIIMOTE_
#include "private/Wiimote_IR_internal.hpp"
#endif

#ifdef _USE_MANYMOUSE_
#include "private/MouseState.hpp"
#endif

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
    :MAX_INPUTS(2), keyboard_mouse_input_(true), inited_(false),
     window_focus_now_(true), window_focus_last_(false), mice_detected_by_manymouse_(0)

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
    for( int i = 0; i < MAX_INPUTS; ++i ) {
        std::string name("player");
        name += (i+1+48); //ascii-code...
        name += ".zzml";
        inputs_.push_back( new Input( "input/" + name ) );
    }
    initGraphicItems();
    return true;
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
#ifdef _USE_MANYMOUSE_
    mice_detected_by_manymouse_ = ManyMouse_Init();
    std::cout << "ManyMouse_Init: " << mice_detected_by_manymouse_ << std::endl;
#endif
}

void InputMgr::quitManyMouse()
{
#ifdef _USE_MANYMOUSE_
    ManyMouse_Quit();
    std::cout << "ManyMouse_Quit." << std::endl;
#endif
}

void InputMgr::pollManyMouseStates()
{
#ifdef _USE_MANYMOUSE_
    ManyMouseEvent e;
    while ( ManyMouse_PollEvent(&e) ) {
        if (e.device >= static_cast<unsigned int>(MAX_INPUTS) )
            continue;

        pMouseState ms;
        for( int i = 0; i < MAX_INPUTS; ++i ) {
            if( inputs_[i]->state_->device_id == e.device )
                ms = inputs_[i]->state_;
        }
        if( !ms ) continue;

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

void InputMgr::handleManyMouseDisconnect()
{
#ifdef _USE_MANYMOUSE_
#endif //_USE_MANYMOUSE_
}

void InputMgr::reinitManyMouse()
{
#ifdef _USE_MANYMOUSE_
    quitManyMouse();
    initManyMouse();

    int total_inputs = (mice_detected_by_manymouse_ > MAX_INPUTS) ? MAX_INPUTS : mice_detected_by_manymouse_;

    for( int i = 0; i < total_inputs; ++i ) {
        pMouseState ms = inputs_[i]->state_;
        ms->name = ManyMouse_DeviceName(i);
        ms->device_id = i;
        ms->connected = true;
        std::cout << "Mice #" << i << ": " << ms->name << std::endl;
    }
#endif //_USE_MANYMOUSE_
}

void InputMgr::updateAll()
{
    pollManyMouseStates();

    MastEventReceiver::i().endEventProcess();

    //toggle keyboard_mouse_input_ on/off
    if( window_focus_now_ )
        if( MastEventReceiver::i().keyPressed( KEY_ESCAPE ) )
            toggleInput(!keyboard_mouse_input_);

    window_focus_last_ = window_focus_now_;
    window_focus_now_ = IrrDevice::i().d()->isWindowActive();

    if( windowGotFocus() ) toggleInput(true);
    else if( windowReleasedFocus() ) toggleInput(false);

#ifdef _USE_MANYMOUSE_
    if( keyboard_mouse_input_ )
        IrrDevice::i().d()->getCursorControl()->setPosition(0.5f, 0.5f); //grab system cursor
#endif //_USE_MANYMOUSE_

    BOOST_FOREACH( Input* it, inputs_ )
        it->update();

    MastEventReceiver::i().startEventProcess();
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
    else
        IrrDevice::i().d()->getCursorControl()->setVisible(true);
}

void InputMgr::initGraphicItems()
{
    if( !scene_ ) {
        scene_ = view::Scene::create("Input scene");
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
     haste_(this), pause_(this), ai_controlled_(false)
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

    CURSOR_SENSATIVITY = static_cast<float>( keymap.I("speed") );

#ifdef _USE_WIIMOTE_
    wiimote_.Connect();
    wiimote_.SetLEDs(wiimote::TotalConnected());
    wiimote_.SetReportType(wiimote::IN_BUTTONS_ACCEL_IR);
    std::cout << "Wiimote connected: " << wiimote::TotalConnected() << std::endl;
#endif //_USE_WIIMOTE_
#ifdef _USE_MANYMOUSE_
    state_ = MouseState::create();
    if( InputMgr::i().count() >= InputMgr::i().manyMouseCount() ) {
        state_->name = "Mouse-like control not found for this Input object.";
        state_->device_id = -1;
        state_->connected = false;
    } else {
        state_->name = ManyMouse_DeviceName( InputMgr::i().count() );
        state_->device_id = InputMgr::i().count();
        state_->connected = true;
    }
    std::cout<< "Mice #" << InputMgr::i().count() << ": " << state_->name << std::endl;
#endif //_USE_MANYMOUSE_

    cursor_texture_name_ = keymap.S("cursor_texture");
    area_texture_name_ = keymap.S("area_rect");
}

void Input::update()
{
    if( ai_controlled_ ) { //AI integration testing
        update_btn_state();
        write_state_now_to_last();
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
#ifdef _USE_MANYMOUSE_
    state_->constrain();
#endif // _USE_MANYMOUSE_
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
    EventDispatcher::i().subscribe_timer( bind(&wiimote::SetRumble, &wiimote_, false), ms );
}
#endif //_USE_WIIMOTE_

void Input::init_graphic()
{
    cursor_mark_ = view::Sprite::create(cursor_texture_name_, InputMgr::i().scene(), 128, 128, true);
    cursor_mark_->tween<Linear, Rotation>(vec3(0,0,360), 3000u, -1);
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

view::pSprite Input::getCursor()
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
    float speed = CURSOR_SENSATIVITY * 150.f / IrrDevice::i().d()->getVideoDriver()->getFPS();
    if( speed < 1.f ) speed = 1.f;
    if( cursor_key_ == 0 ) {     //A Special Case Which Is Not EKEY_CODE: 0x00 means "mouse"
#ifdef _USE_MANYMOUSE_
        cursor_.x() = state_->x;
        cursor_.y() = state_->y;
#else // _USE_MANYMOUSE_
        cursor_.x() = MastEventReceiver::i().mouseX();
        cursor_.y() = MastEventReceiver::i().mouseY();
#endif // _USE_MANYMOUSE_
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
#ifdef _USE_MANYMOUSE_
    unsigned int button_bits = state_->buttons;
    if( cursor_key_ == 0 ) {
        trig1_.now() = (button_bits & trig1_key_) > 0;
        trig2_.now() = (button_bits & trig2_key_) > 0;
        haste_.now() = (button_bits & haste_key_) > 0;
    } else {
        trig1_.now() = MastEventReceiver::i().keyDown( trig1_key_ );
        trig2_.now() = MastEventReceiver::i().keyDown( trig2_key_ );
        haste_.now() = MastEventReceiver::i().keyDown( haste_key_ );
    }
#else // _USE_MANYMOUSE_
    trig1_.now() = MastEventReceiver::i().keyDown( trig1_key_ );
    trig2_.now() = MastEventReceiver::i().keyDown( trig2_key_ );
    haste_.now() = MastEventReceiver::i().keyDown( haste_key_ );
#endif // _USE_MANYMOUSE_
    wep1_.now() = MastEventReceiver::i().keyDown( wep1_key_ );
    wep2_.now() = MastEventReceiver::i().keyDown( wep2_key_ );
    wep3_.now() = MastEventReceiver::i().keyDown( wep3_key_ );
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
    std::cout << "Input destructing ..." << std::endl;
}
