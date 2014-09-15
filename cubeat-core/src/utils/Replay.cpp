
#include "utils/Replay.hpp"
#include "utils/to_s.hpp"

#include "Accessors.hpp"
#include "EasingEquations.hpp"
#include "view/Sprite.hpp"
#include "Input.hpp"
#include "Player.hpp"
#include "presenter/game/Demo.hpp"
#include "ctrl/TimerDispatcher.hpp"
#include "Conf.hpp"

using namespace psc;
using namespace utils;

Replay::Replay():
    recording_(false), replaying_(false), p1t1_(false), p1t2_(false), p2t1_(false), p2t2_(false),
    x1_(0), y1_(0), x2_(0), y2_(0)
{
    replay_data_["frametime"] = vector_any();
    replay_data_["input"]     = map_any();
}

void Replay::set_timer_dispatcher(ctrl::wpTimerDispatcher t)
{
    game_timer_ = t;
}

void Replay::seed(int const& seed)
{
    replay_data_["seed"] = seed;
}

int Replay::seed() const
{
    if( !replay_data_.empty() ) {
        return replay_data_.I("seed");
    } else {
        printf("Replay seed data is bad.\n");
        return 0;
    }
}

void Replay::set_time()
{
    //using the get_curr_tickcount() as index has one very important assumption,
    //which is right before you start recording the replay, you set game timer to 0
    //(when set_time(0) the tickcount will be zeroed as well)
    if( ctrl::pTimerDispatcher td = game_timer_.lock() ) {
        if( replaying_ && !td->is_stopped() ) {
            if( td->get_curr_tickcount() < static_cast<int>(replay_data_.V("frametime").size()) ) {
                td->set_time( replay_data_.V("frametime").I( td->get_curr_tickcount()-1 ) );
            } else {
                printf("Replay data frametime(tick %d) out of bound.\n", td->get_curr_tickcount());
            }
        }
    }
}

void Replay::set_input_for(ctrl::Input* input)
{
    if( ctrl::pTimerDispatcher td = game_timer_.lock() ) {
        if( replaying_ && !td->is_stopped() ) {
            if( input->player()->id() == 0 ) {
                input->trig1().now() = p1t1_;
                input->trig2().now() = p1t2_;
                input->cursor().x() = x1_;
                input->cursor().y() = y1_;
                input->getCursor()->set<accessor::Pos2D>(vec2(x1_, y1_));
            } else {
                input->trig1().now() = p2t1_;
                input->trig2().now() = p2t2_;
                input->cursor().x() = x2_;
                input->cursor().y() = y2_;
                input->getCursor()->set<accessor::Pos2D>(vec2(x2_, y2_));
            }
        }
    }
}

void Replay::replay_advance()
{
    if( ctrl::pTimerDispatcher td = game_timer_.lock() ) {
        if( replaying_ && !td->is_stopped() ) {

            utils::map_any::iterator it_input_data = replay_data_.M("input").find( td->get_curr_tickcount() );

            if( it_input_data != replay_data_.M("input").end() ) {
                //printf(" Replay: current frame: %d, current time: %ld\n", td->get_curr_tickcount(), td->get_time() );
                utils::vector_any input_data = boost::any_cast<utils::vector_any const>((*it_input_data).second);

                bool btn_tmp[2][2] = { {p1t1_, p1t2_} , {p2t1_, p2t2_} };
                int  xy_tmp[2][2] = { {x1_, y1_}, {x2_, y2_} };

                for( unsigned int i = 0; i < input_data.size(); ++i ) {
                    int pid = input_data.V(i).I(0);
                    xy_tmp[pid][0] = input_data.V(i).I(1);
                    xy_tmp[pid][1] = input_data.V(i).I(2);
                    printf(" -- replaying input id: %d  x: %d  y: %d\n", input_data.V(i).I(0), input_data.V(i).I(1), input_data.V(i).I(2));

                    int bid = input_data.V(i).V(3).I(0);
                    btn_tmp[pid][bid] = input_data.V(i).V(3).I(1) == ctrl::BTN_PRESS ? true : false;
                    printf(" -- -- button: %d, down: %d\n", input_data.V(i).V(3).I(0), btn_tmp[pid][bid]);

                    if( input_data.V(i).size() == 5 ) {
                        int bid2 = input_data.V(i).V(4).I(0);
                        btn_tmp[pid][bid2] = input_data.V(i).V(4).I(1) == ctrl::BTN_PRESS ? true : false;
                        printf(" -- -- button: %d, down: %d\n", input_data.V(i).V(4).I(0), btn_tmp[pid][bid2]);
                    }
                }

                p1t1_ = btn_tmp[0][0], p1t2_ = btn_tmp[0][1], p2t1_ = btn_tmp[1][0], p2t2_ = btn_tmp[1][1];
                x1_   = xy_tmp [0][0],   y1_ = xy_tmp [0][1],   x2_ =  xy_tmp[1][0],   y2_ =  xy_tmp[1][1];
            }
        }
    }
}

void Replay::record_frame_time()
{
    if( ctrl::pTimerDispatcher td = game_timer_.lock() ) {
        if( recording_ && !td->is_stopped() ) {
            int t = td->get_time();
            replay_data_.V("frametime").push_back(t);
        }
    }
}

void Replay::record_input_state(ctrl::Input* input) {
    if( ctrl::pTimerDispatcher td = game_timer_.lock() ) {
        if( recording_ && !td->is_stopped() ) {
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
            int current_tick_num = replay_data_.V("frametime").size();// - 1; // starts from 0
            bool we_have_input_this_frame = false;
            vector_any input_data;

            input_data.push_back(input->player()->id());
            input_data.push_back(input->cursor().x());
            input_data.push_back(input->cursor().y());

            if( input->trig1().changed() ) {
                we_have_input_this_frame = true;
                input_data.push_back( vector_any() );
                input_data.V(3).push_back( 0 );                                      // button_id
                input_data.V(3).push_back(static_cast<int>(input->trig1().state())); // button state
            }
            if( input->trig2().changed() ) {
                we_have_input_this_frame = true;
                input_data.push_back( vector_any() );
                int n = input_data.size()-1; // index here may be 3 or 4.
                input_data.V( n ).push_back( 1 );                                      // button_id
                input_data.V( n ).push_back(static_cast<int>(input->trig2().state())); // button state
            }

            if( we_have_input_this_frame ) {
                if( replay_data_.M("input").find(current_tick_num) == replay_data_.M("input").end() ) {
                    replay_data_.M("input").insert(current_tick_num, vector_any());
                }
                replay_data_.M("input").V(current_tick_num).push_back(input_data);
                printf(" tick: %d, time: %d, pid: %d, x: %d  y: %d\n", current_tick_num, replay_data_.V("frametime").I(current_tick_num-1), input_data.I(0), input_data.I(1), input_data.I(2) );
                printf(" -- button: %d, down: %d\n", input_data.V(3).I(0), input_data.V(3).I(1));
                if( input_data.size() == 5 ) {
                    printf(" -- button: %d, down: %d\n", input_data.V(4).I(0), input_data.V(4).I(1));
                }
            }
        }
    }
}

void Replay::toggle_recording_andor_replaying(bool const& f, presenter::game::pDemo const& demo) {
    if( f ) {
        if( replay_data_.V("frametime").size() == 0 ) {
            recording_ = true;
        } else {
            replaying_ = true;
        }
    } else {
        if( recording_ ) {
            recording_ = false;
            // write additional metadata
            replay_data_["map0_score"] = demo->get_map_score(0);
            replay_data_["map1_score"] = demo->get_map_score(1);
            Conf::i().save_config(replay_data_, "tmp/replay");
            reset_data();
        }
        if( replaying_ ) {
            ctrl::pTimerDispatcher td = game_timer_.lock();
            time_t t = replay_data_.V("frametime").I(replay_data_.V("frametime").size()-1);
            if( td->get_time() != t ) {
                printf("Replay: record time %d mismatches with current time %d\n", t, td->get_time());
            }
            if( replay_data_.I("map0_score") != demo->get_map_score(0) ) {
                printf("Replay: map0_score mispatch: record %d, current %d\n", replay_data_.I("map0_score"), demo->get_map_score(0));
            }
            if( replay_data_.I("map1_score") != demo->get_map_score(1) ) {
                printf("Replay: map1_score mispatch: record %d, current %d\n", replay_data_.I("map1_score"), demo->get_map_score(1));
            }
            replaying_ = false;
        }
        p1t1_ = false, p1t2_ = false, p2t1_ = false, p2t2_ = false;
    }
}

bool Replay::read_file(std::string const& path) {
    replay_data_ = Conf::i().config_of(path);
    if( replay_data_.empty() ) {
        printf("Replay data not found: %s\n", path.c_str());
        reset_data();
        return false;
    }
    return true;
}

void Replay::reset_data() {
    replay_data_.clear();
    replay_data_["frametime"] = vector_any();
    replay_data_["input"]     = map_any();
}
