
#include "IrrDevice.hpp"
#include "utils/Timer.hpp"
#include "ctrl/TimerDispatcher.hpp"

#include <boost/foreach.hpp>
#include <boost/tr1/functional.hpp>
#include <algorithm>
#include <utility>

using std::tr1::tie;
using std::tr1::get;
using std::tr1::weak_ptr;
using std::tr1::shared_ptr;
using std::tr1::bind;
using namespace std::tr1::placeholders;

using namespace psc;
using namespace ctrl;

TimerDispatcher::TimerDispatcher(std::string const& name):frame_counter_(0), old_frame_counter_(0), name_(name) {}
TimerDispatcherNormal::TimerDispatcherNormal(std::string const& name): TimerDispatcher(name) {}
TimerDispatcherRestorable::TimerDispatcherRestorable(std::string const& name): TimerDispatcher(name) {}

TimerDispatcher::~TimerDispatcher() { delete timer_; }
TimerDispatcherNormal::~TimerDispatcherNormal() { printf("TimerDispatcherNormal died: %s\n", name_.c_str()); }
TimerDispatcherRestorable::~TimerDispatcherRestorable() { printf("TimerDispatcherRestorable died: %s\n", name_.c_str()); }

// timer_ start on running by default
void TimerDispatcher::init() { timer_ = new utils::Timer(IrrDevice::i().d()->getTimer(), true); }

TimerDispatcherNormal::pointer_type TimerDispatcherNormal::init() {
    TimerDispatcher::init();
    self_ = std::tr1::static_pointer_cast<TimerDispatcherNormal>( shared_from_this() );
    return self_.lock();
}

TimerDispatcherRestorable::pointer_type TimerDispatcherRestorable::init() {
    TimerDispatcher::init();
    self_ = std::tr1::static_pointer_cast<TimerDispatcherRestorable>( shared_from_this() );
    return self_.lock();
}

TimerDispatcherNormal& TimerDispatcherNormal::subscribe
    (TimerDispatcher::TimerCallback const& cb, wpvoid const& obj, int const& duration, int const& loop)
{
    int const zero = 0;
    newly_created_timers_.push_back( tie( cb, duration, zero, loop, obj ) );
    return *this;
}

TimerDispatcherNormal& TimerDispatcherNormal::subscribe
    (TimerDispatcher::TimerCallback const& cb, int const& duration, int const& loop)
{
    return subscribe(cb, self_, duration, loop);
}

TimerDispatcherRestorable& TimerDispatcherRestorable::subscribe
    (TimerDispatcher::TimerCallback const& cb, wpvoid const& obj, int const& duration, int const& loop)
{
    int const zero = 0;
    newly_created_timers_.push_back( tie( cb, duration, zero, loop, obj ) );
    return *this;
}

TimerDispatcherRestorable& TimerDispatcherRestorable::subscribe
    (TimerDispatcher::TimerCallback const& cb, int const& duration, int const& loop)
{
    return subscribe(cb, self_, duration, loop);
}

//Ok, since the following note says it has bug don't use, and it really is not used anywhere for now
//I am going to leave _clear_timer_event_restorable_ unimplemented.

//note: has bug, don't use.
TimerDispatcher& TimerDispatcher::clear_timer_event()
{
//    for(TimerList::iterator t = timers_.begin(), tend = timers_.end(); t != tend; ++t)
//        timers_to_be_deleted_.push_back(t);
//
//    // clean up
//    BOOST_FOREACH(TimerList::iterator t, timers_to_be_deleted_) {
//        timers_.erase(t);
//    }
//    timers_to_be_deleted_.clear();
//    timers_.clear();
    return *this;
}
TimerDispatcherNormal& TimerDispatcherNormal::clear_timer_event() {}
TimerDispatcherRestorable& TimerDispatcherRestorable::clear_timer_event() {}

TimerDispatcher& TimerDispatcher::stop()
{
    timer_->stop();
    return *this;
}

TimerDispatcher& TimerDispatcher::start()
{
    timer_->start();
    return *this;
}

TimerDispatcher& TimerDispatcher::set_speed(double const& factor)
{
    timer_->setSpeed(factor);
    return *this;
}

TimerDispatcher& TimerDispatcher::set_time(time_t const& t)
{
    timer_->setTime(t);
    return *this;
}

double TimerDispatcher::get_speed() const
{
    return timer_->getSpeed();
}

bool TimerDispatcher::is_stopped() const
{
    return timer_->isStopped();
}

std::time_t TimerDispatcher::get_time() const
{
    return timer_->getTime();
}

TimerDispatcher& TimerDispatcher::set_frame(int const& frame) {
    frame_counter_ = frame;
    return *this;
}

void TimerDispatcherNormal::tick()
{
    timer_->tick();
}

void TimerDispatcherRestorable::tick()
{
    // WTF MEMO: This method actually will get more complicated after we factor in
    // the need of synchronizing 2 different clocks on 2 different machines.
    // On the slave machine we can't use the locally ticked time, we have to use the
    // time that comes with the remote input through networking.

    if( frame_counter_ == old_frame_counter_ + 1 ) { // corrected frame
        timer_->tick();
        recorded_times_.push_back( timer_->getTime() );
        if( recorded_times_.size() > 60 ) {
            recorded_times_.pop_front();
        }
        old_frame_counter_ = frame_counter_; // is it ok to refresh old_frame_counter here? any exceptions?
    } else {
        int frame_of_rb = old_frame_counter_ - frame_counter_ + 1;
        if( frame_of_rb <= static_cast<int>(recorded_times_.size()) ) {
            time_t const& t = recorded_times_[ recorded_times_.size() - frame_of_rb ];
            timer_->setTime(t);
        }
        else {
            printf("Reversible Timer: time records underflow.\n");
        }
    }
}

/// This is the Main Loop of Timer

void TimerDispatcherNormal::dispatch()
{
    cleanup_timer_and_init_newly_created_timer();

    std::time_t now = timer_->getTime();
    for(TimerList::iterator t = timers_.begin(), tend = timers_.end(); t != tend; ++t) {
        if( get<TimerDispatcher::CALLEE>(*t).lock() ) {
            if( now - get<TimerDispatcher::LASTTIME>(*t) >= get<TimerDispatcher::DURATION>(*t) ) {

                //debug
//                if( get<TE::DURATION>(*t) == 1000 && get<TE::LOOP>(*t) > 0 ) {
//                    std::cout << "   timer: functor " << get<TE::CALLEE>(*t).lock() << ", reg-ed duration: " << get<TE::DURATION>(*t) << " loop: " << get<TE::LOOP>(*t) << std::endl;
//                    int* a = reinterpret_cast<int*>((get<TE::CALLEE>(*t).lock().get()));
//                    std::cout << "   " << *a << std::endl;
//                }

                get<TimerDispatcher::TIMER_CB>(*t)();

                //debug
//                if( get<TE::DURATION>(*t) == 1000 && get<TE::LOOP>(*t) > 0 )
//                    std::cout << "   timer: functor " << get<TE::CALLEE>(*t).lock() << std::endl;

                get<TimerDispatcher::LASTTIME>(*t) = now; //get<TE::LASTTIME>(*t) += get<TE::DURATION>(*t);
                int& looptimes = get<TimerDispatcher::LOOP>(*t);
                if( looptimes == 0 ) {
                    timers_to_be_deleted_.push_back(t);
                } else if( looptimes > 0 ) looptimes -= 1;
            }
        }
        else timers_to_be_deleted_.push_back(t);
    }
    // clean up
    BOOST_FOREACH(TimerList::iterator t, timers_to_be_deleted_) {
        timers_.erase(t);
    }
    timers_to_be_deleted_.clear();
}

void TimerDispatcherRestorable::dispatch()
{
    // It's just the same as the non-restorable counterpart just without all the commented lines.
    cleanup_timer_and_init_newly_created_timer();

    std::time_t now = timer_->getTime();
    for(TimerList::iterator t = timers_.begin(), tend = timers_.end(); t != tend; ++t) {
        if( get<TimerDispatcher::CALLEE>(*t).lock() ) {
            if( now - get<TimerDispatcher::LASTTIME>(*t) >= get<TimerDispatcher::DURATION>(*t) ) {
                get<TimerDispatcher::TIMER_CB>(*t)();
                get<TimerDispatcher::LASTTIME>(*t) = now;
                int& looptimes = get<TimerDispatcher::LOOP>(*t);
                if( looptimes == 0 ) {
                    timers_to_be_deleted_.push_back(t);
                } else if( looptimes > 0 ) looptimes -= 1;
            }
        }
        else timers_to_be_deleted_.push_back(t);
    }
    // clean up
    BOOST_FOREACH(TimerList::iterator t, timers_to_be_deleted_) {
        timers_.erase(t);
    }
    timers_to_be_deleted_.clear();

}

/// Cleanup methods of Timer

void TimerDispatcherNormal::cleanup_timer_and_init_newly_created_timer()
{
    // init newly created
    std::time_t init_time = timer_->getTime();
    BOOST_FOREACH(Timer& timer, newly_created_timers_){
        get<TimerDispatcher::LASTTIME>(timer) = init_time;
    }

    timers_.insert(timers_.end(),
                   newly_created_timers_.begin(),
                   newly_created_timers_.end());
    newly_created_timers_.clear();
}

void TimerDispatcherRestorable::cleanup_timer_and_init_newly_created_timer()
{
    // init newly created
    std::time_t init_time = timer_->getTime();
    BOOST_FOREACH(Timer& timer, newly_created_timers_){
        get<TimerDispatcher::LASTTIME>(timer) = init_time;
    }

    timers_.insert(timers_.end(),
                   newly_created_timers_.begin(),
                   newly_created_timers_.end());
    newly_created_timers_.clear();
}
