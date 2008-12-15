#ifndef _SHOOTING_CUBES_INPUT_CROSSHAIR_
#define _SHOOTING_CUBES_INPUT_CROSSHAIR_

class Input;

class Crosshair
{
public:
    int const& x() const { return x_; }
    int const& y() const { return y_; }
    int const& lx() const { return lx_; }
    int const& ly() const { return ly_; }
    int const width() const { return rightbound_ - leftbound_; }
    int const height()const { return bottombound_ - topbound_; }
    bool const changed() const { return (lx_ != x_ || ly_ != y_); }

    Input const* owner() const { return owner_; }

    int& x() { return x_; }
    int& y() { return y_; }
    
    void write_state_now_to_last() { lx_ = x_; ly_ = y_; }
    void constrain();

    Crosshair(Input const*);

private:
    int x_, y_;
    int lx_, ly_;
    int leftbound_;
    int rightbound_;
    int topbound_;
    int bottombound_;

    Input const* owner_;
};

#endif