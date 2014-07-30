#ifndef _SHOOTING_CUBES_UTILS_REPLAY_
#define _SHOOTING_CUBES_UTILS_REPLAY_

#include "utils/dictionary.hpp"
#include "ButtonEnum.hpp"

namespace psc {
namespace utils {

class Replay
{
public:
    void record_frame_time();
    void record_input_state();
    void seed(int const& seed);
    void reset_data();
    void toggle_enable(bool const& f);
    bool is_enabled() const { return enabled_; }

    Replay();

private:
    bool enabled_;
    map_any replay_data_;
};

} //utils
} //psc

#endif //_SHOOTING_CUBES_UTILS_REPLAY_

