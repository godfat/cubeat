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

#ifdef _USE_MANYMOUSE_
class MouseState;
typedef std::tr1::shared_ptr<MouseState> pMouseState;
typedef std::tr1::weak_ptr<MouseState>  wpMouseState;
#endif //_USE_MANYMOUSE_

class InputMgr
{
    typedef std::vector<Input*>      InputList;

public:
    static InputMgr& i() {
        static InputMgr singleton;
        return singleton;
    }

    ~InputMgr();
    bool createInputs();
    Input* getInputByIndex(unsigned int);
    InputList& getInputs() { return inputs_; }
    view::pScene& scene()  { return scene_; }
    int count() const      { return static_cast<int>(inputs_.size()); }
    bool keyboardMouseInput() const { return keyboard_mouse_input_; }
    bool inited()        const { return inited_; }
    bool manyMouseCount() const { return mice_detected_by_manymouse_; }
    void updateAll(); //we can't combine updateAll and redrawAll,
    void redrawAll(); //because we must update input before everything in the main loop,
                      //however redraw input after everything (so the cursors won't be covered)

    int const MAX_INPUTS;

private:
    InputMgr();
    InputMgr(InputMgr const&); //singleton don't implement this
    bool cleanupInputs();
    void initGraphicItems();
    void initManyMouse();
    void quitManyMouse();
    void pollManyMouseStates();
    void handleManyMouseDisconnect();
    void reinitManyMouse();

    view::pScene scene_;
    InputList    inputs_;
    bool keyboard_mouse_input_;
    bool inited_;
    int  mice_detected_by_manymouse_;
};

class Input
{
    friend class InputMgr;

public:
    Input(std::string const& path);
    ~Input();

    Crosshair const& cursor() const { return cursor_; }
    Crosshair& cursor() { return cursor_; }
    Button const& trig1() const { return trig1_; }
    Button const& trig2() const { return trig2_; }
    Button const& wep1() const { return wep1_; }
    Button const& wep2() const { return wep2_; }
    Button const& wep3() const { return wep3_; }
    Button const& haste() const { return haste_; }
    Button const& pause() const { return pause_; }

//<AI integration testing>
    Button& trig1() { return trig1_; }
    Button& trig2() { return trig2_; }
    Button& wep1() { return wep1_; }
    Button& wep2() { return wep2_; }
    Button& wep3() { return wep3_; }
    Button& haste() { return haste_; }
    Button& pause() { return pause_; }
//</AI integration testing>

    Input&  player(wpPlayer player);
    pPlayer player() const;
    void setRangeShapeVisible(bool);
    void setCursorVisible(bool);
    view::pSprite getCursor();

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

//<AI integration testing>
public:
    bool isControlledByAI() const { return ai_controlled_; }
    void setControlledByAI(bool const& flag) { ai_controlled_ = flag; }
private:
    bool ai_controlled_;
//</AI integration testing>

#ifdef _USE_WIIMOTE_
public:
    void rumbleWiimote(int ms);
private:
    void update_by_wiimote();
    wiimote wiimote_;
#endif
#ifdef _USE_MANYMOUSE_
    pMouseState state_;
#endif
};

}  //ctrl
}  //psc

#endif
