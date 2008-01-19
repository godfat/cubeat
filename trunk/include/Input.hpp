#ifndef _SHOOTING_CUBES_INPUT_INTERFACE_
#define _SHOOTING_CUBES_INPUT_INTERFACE_

/* Input header/class-definition,
   Undefine _USE_WIIMOTE_ to compile a keyboard/mouse-only version.

   reads keyboard/mouse message or wiimote state and convert them
   into a internal and unified button state structure.

   TODO:
   * Nothing for now.
*/

#include "Crosshair.hpp"
#include "Button.hpp"

#include <string>
#include <vector>
#include <map>

#ifdef _USE_WIIMOTE_
#include <wiimote.h>
#endif

namespace psc { namespace ctrl {

class Input
{
//static things here
    typedef std::vector<Input*> InputList;
public:
    static void update_all();
    static Input* getInputByIndex(unsigned int);
    static InputList& getInputs() { return inputs_; }
    static int count() { return static_cast<int>(inputs_.size()); }

private:
    static InputList inputs_;
    static bool keyboard_mouse_input_;

    static const int MAX_SETTINGS = 8;
    static const int CURSOR_SENSATIVITY = 5;

//non-static below
public:
    Input(char const*);
    ~Input();

    Crosshair const& cursor() const { return cursor_; }
    Button const& trig1() const { return trig1_; }
    Button const& trig2() const { return trig2_; }
    Button const& wep1() const { return wep1_; }
    Button const& wep2() const { return wep2_; }
    Button const& wep3() const { return wep3_; }
    Button const& haste() const { return haste_; }
    Button const& pause() const { return pause_; }

private:
    Input(Input const&);    //No copy constructor

    void update();
    void cursor_by_keyboard_mouse();
    void buttons_by_keyboard_mouse();
    void write_state_now_to_last();
    void update_btn_state();

    Crosshair cursor_;
    Button trig1_;
    Button trig2_;
    Button wep1_;
    Button wep2_;
    Button wep3_;
    Button haste_;
    Button pause_;

    typedef std::pair<std::string, int> ItemKey;
    std::map<ItemKey::first_type, ItemKey::second_type> keymap;

#ifdef _USE_WIIMOTE_
    wiimote wiimote_;
#endif
};

}  //ctrl
}  //psc

#endif
