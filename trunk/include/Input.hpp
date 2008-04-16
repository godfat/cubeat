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
#include <tr1/memory>

#ifdef _USE_WIIMOTE_
#include <wiimote.h>
#endif

namespace psc {

namespace view {
class Scene;
typedef std::tr1::shared_ptr<Scene> pScene;
typedef std::tr1::weak_ptr<Scene> wpScene;
class Sprite;
typedef std::tr1::shared_ptr<Sprite> pSprite;
typedef std::tr1::weak_ptr<Sprite> wpSprite;
}

namespace ctrl {

class Player;
typedef std::tr1::shared_ptr<Player> pPlayer;
typedef std::tr1::weak_ptr<Player> wpPlayer;

class Input
{
//static things here
    typedef std::vector<Input*> InputList;
public:
    static void update_all();
    static void redraw_all();
    static void init_graphic_items();
    static Input* getInputByIndex(unsigned int);
    static InputList& getInputs() { return inputs_; }
    static int count() { return static_cast<int>(inputs_.size()); }
    static view::pScene& scene() { return scene_; } //note: bad

private:
    static view::pScene scene_;
    static InputList inputs_;
    static bool keyboard_mouse_input_;

//non-static below
public:
    Input(std::string const& path);
    ~Input();

    Crosshair const& cursor() const { return cursor_; }
    Button const& trig1() const { return trig1_; }
    Button const& trig2() const { return trig2_; }
    Button const& wep1() const { return wep1_; }
    Button const& wep2() const { return wep2_; }
    Button const& wep3() const { return wep3_; }
    Button const& haste() const { return haste_; }
    Button const& pause() const { return pause_; }

    Input&  player(wpPlayer player);
    pPlayer player() const;
    void setRangeShapeVisible(bool);
    void setCursorVisible(bool);

private:
    Input(Input const&);    //No copy constructor

    void update();
    void redraw();
    void cursor_by_keyboard_mouse();
    void buttons_by_keyboard_mouse();
    void write_state_now_to_last();
    void update_btn_state();
    void init_graphic();

    Crosshair cursor_;
    Button trig1_;
    Button trig2_;
    Button wep1_;
    Button wep2_;
    Button wep3_;
    Button haste_;
    Button pause_;

    int cursor_key_;
    int trig1_key_;
    int trig2_key_;
    int wep1_key_;
    int wep2_key_;
    int wep3_key_;
    int haste_key_;
    int pause_key_;

    float CURSOR_SENSATIVITY;

    wpPlayer player_;
    view::pSprite cursor_mark_;
    view::pSprite range_shape_;
    std::string cursor_texture_name_;
    std::string area_texture_name_;
#ifdef _USE_WIIMOTE_
    wiimote wiimote_;
#endif
};

}  //ctrl
}  //psc

#endif
