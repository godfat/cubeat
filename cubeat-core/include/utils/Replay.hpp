#ifndef _SHOOTING_CUBES_UTILS_REPLAY_
#define _SHOOTING_CUBES_UTILS_REPLAY_

#include "utils/dictionary.hpp"
#include "ButtonEnum.hpp"
#include <boost/tr1/memory.hpp>

namespace psc {

namespace ctrl {
class Input;
class TimerDispatcher;
typedef std::tr1::shared_ptr<TimerDispatcher> pTimerDispatcher;
typedef std::tr1::weak_ptr<TimerDispatcher> wpTimerDispatcher;
}

namespace utils {

class Replay
{
public:
    void record_frame_time();
    void record_input_state(ctrl::Input* input);
    void set_timer_dispatcher(ctrl::wpTimerDispatcher t);
    void set_time();
    void set_input_for(ctrl::Input* input);
    void replay_advance();
    void seed(int const& seed);
    int  seed() const;
    bool read_file(std::string const& path);
    void toggle_recording_andor_replaying(bool const& f);
    void reset_data();
    bool is_recording() const { return recording_; }
    bool is_replaying() const { return replaying_; }

    Replay();

private:
    bool recording_, replaying_;
    bool p1t1_, p1t2_, p2t1_, p2t2_;
    int x1_, y1_, x2_, y2_;
    map_any replay_data_;
    ctrl::wpTimerDispatcher game_timer_;
};

} //utils
} //psc

#endif //_SHOOTING_CUBES_UTILS_REPLAY_

