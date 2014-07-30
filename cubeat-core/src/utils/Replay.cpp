
#include "utils/Replay.hpp"
#include "utils/to_s.hpp"
#include "Input.hpp"
#include "ctrl/TimerDispatcher.hpp"
#include "EventDispatcher.hpp"
#include "Conf.hpp"

using namespace psc;
using namespace utils;

Replay::Replay(): enabled_(false) {
    replay_data_["frametime"] = vector_any();
    replay_data_["input"]     = map_any();
}

void Replay::seed(int const& seed) {
    replay_data_["seed"] = seed;
}

void Replay::record_frame_time() {
    if( enabled_ && !ctrl::EventDispatcher::i().get_timer_dispatcher("game")->is_stopped() ) {
        int t = ctrl::EventDispatcher::i().get_timer_dispatcher("game")->get_time();
        replay_data_.V("frametime").push_back(t);
    }
}

void Replay::record_input_state() {
    if( enabled_ && !ctrl::EventDispatcher::i().get_timer_dispatcher("game")->is_stopped() ) {
        // Because we want to keep the internals of Input clean, this recording addition should be polled
        // inside Replay class and not calling a lot of Replay methods inside Input class.

        /* Note: the replay input data structure:

        <tick_num> : [
            [ <input_id>, <x>, <y>, [<button_id>, <button_state>], [<button_id>, <button_state>] ],
            [ <input_id>, <x>, <y>, [<button_id>, <button_state>], [...] ],
        ],

        avoid using map_any because map_any is slower and also take up more literal space
        and when replaying the data I can simply walk through the data set serialy,
        without the need to map.find("name") == map.end() when ever I want to check anything.

        */

        // try to poll inputs
        int current_tick_num = replay_data_.V("frametime").size() - 1; // starts from 0
        bool we_have_input_this_frame = false;
        vector_any frame_input_data;

        for( int i = 0; i < static_cast<int>(ctrl::InputMgr::i().getInputs().size()); ++i ) {
            ctrl::Input* input = ctrl::InputMgr::i().getInputByIndex(i);
            bool this_input_is_changed_this_frame = false;
            vector_any input_data;
            input_data.push_back(i); // input_id
            input_data.push_back(input->cursor().x());
            input_data.push_back(input->cursor().y());
            if( input->trig1().changed() ) {
                we_have_input_this_frame = true;
                this_input_is_changed_this_frame = true;
                input_data.push_back( vector_any() );
                // index here must be 3.
                input_data.V(3).push_back( 0 );                                      // button_id
                input_data.V(3).push_back(static_cast<int>(input->trig1().state())); // button state
            }
            if( input->trig2().changed() ) {
                we_have_input_this_frame = true;
                this_input_is_changed_this_frame = true;
                input_data.push_back( vector_any() );
                int n = input_data.size()-1; // index here may be 3 or 4.
                input_data.V( n ).push_back( 1 );                                      // button_id
                input_data.V( n ).push_back(static_cast<int>(input->trig2().state())); // button state
            }

            if( this_input_is_changed_this_frame ) {
                printf("%d %d %d\n", input_data.I(0), input_data.I(1), input_data.I(2));
                frame_input_data.push_back( input_data );
            }
        }

        if( we_have_input_this_frame ) {
            replay_data_.M("input")[current_tick_num] = frame_input_data;
        }
    }
}

void Replay::toggle_enable(bool const& f) {
    if( !f && enabled_ ) {
        Conf::i().save_config(replay_data_, "tmp/replay");
        reset_data();
    }
    enabled_ = f;
}

void Replay::reset_data() {
    replay_data_.clear();
    replay_data_["frametime"] = vector_any();
    replay_data_["input"]     = map_any();
}
