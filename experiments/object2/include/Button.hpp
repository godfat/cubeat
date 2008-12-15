#ifndef _SHOOTING_CUBES_BUTTON_STATE_
#define _SHOOTING_CUBES_BUTTON_STATE_

namespace ctrl {

enum BSTATE{
    BTN_UP = 0,
    BTN_DOWN = 1,
    BTN_PRESS = 2,
    BTN_RELEASE = 3
};

class Input;

class Button
{
public:
    Button(Input const* input);

    bool const& now() const { return now_; }
    bool const& last()const { return last_; }
    BSTATE const& state() const { return state_; }
    bool const pressed() const { return (last_ == false && now_ == true); }
    bool const released()const { return (last_ == true && now_ == false); }
    bool const changed() const { return last_ != now_; }

    Input const* owner() const { return owner_; }

    bool& now() { return now_; }

    void write_state_now_to_last() { last_ = now_; }
    void update_state() {
        if( pressed() )       state_ = BTN_PRESS;
        else if( released() ) state_ = BTN_RELEASE;
        else if( now() )      state_ = BTN_DOWN;
        else                  state_ = BTN_UP;
    }

private:
    bool now_, last_;
    BSTATE state_;
    Input const* owner_;
};

} //ctrl

#endif
