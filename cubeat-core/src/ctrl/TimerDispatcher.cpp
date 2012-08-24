
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

TimerDispatcher::TimerDispatcher(std::string const& name)
    :name_(name)
{
    printf("TimerDispatcher created: %s\n", name_.c_str());
}

TimerDispatcher::~TimerDispatcher()
{
    printf("TimerDispatcher died: %s\n", name_.c_str());
    delete timer_;
}

pTimerDispatcher TimerDispatcher::init()
{
    self_  = shared_from_this();
    timer_ = new utils::Timer(IrrDevice::i().d()->getTimer(), true); //start on running by default
    return self_.lock();
}

TimerDispatcher& TimerDispatcher::subscribe
    (TimerCallback const& cb, wpvoid const& obj, int const& duration, int loop)
{
    int const zero = 0;
    newly_created_timers_.push_back( tie( cb, duration, zero, loop, obj ) );
    return *this;
}

TimerDispatcher& TimerDispatcher::subscribe
    (TimerCallback const& cb, int const& duration, int loop)
{
    return subscribe(cb, shared_from_this(), duration, loop);
}


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

void TimerDispatcher::tick()
{
    timer_->tick();
}

/// This is the Main Loop of Timer

void TimerDispatcher::dispatch()
{
    cleanup_timer_and_init_newly_created_timer();

    std::time_t now = timer_->getTime();
    for(TimerList::iterator t = timers_.begin(), tend = timers_.end(); t != tend; ++t) {
        if( get<TE::CALLEE>(*t).lock() ) {
            if( now - get<TE::LASTTIME>(*t) >= get<TE::DURATION>(*t) ) {

                //debug
//                if( get<TE::DURATION>(*t) == 1000 && get<TE::LOOP>(*t) > 0 ) {
//                    std::cout << "   timer: functor " << get<TE::CALLEE>(*t).lock() << ", reg-ed duration: " << get<TE::DURATION>(*t) << " loop: " << get<TE::LOOP>(*t) << std::endl;
//                    int* a = reinterpret_cast<int*>((get<TE::CALLEE>(*t).lock().get()));
//                    std::cout << "   " << *a << std::endl;
//                }

                get<TE::TIMER_CB>(*t)();

                //debug
//                if( get<TE::DURATION>(*t) == 1000 && get<TE::LOOP>(*t) > 0 )
//                    std::cout << "   timer: functor " << get<TE::CALLEE>(*t).lock() << std::endl;

                get<TE::LASTTIME>(*t) = now; //get<TE::LASTTIME>(*t) += get<TE::DURATION>(*t);
                int& looptimes = get<TE::LOOP>(*t);
                if( looptimes == 0 ) {
                    timers_to_be_deleted_.push_back(t);
                } else if( looptimes > 0 ) looptimes -= 1;
            }
        }
        else timers_to_be_deleted_.push_back(t);
    }
}

/// Cleanup methods of Timer

void TimerDispatcher::cleanup_timer_and_init_newly_created_timer()
{
    // clean up
    BOOST_FOREACH(TimerList::iterator t, timers_to_be_deleted_) {
        timers_.erase(t);
    }
    timers_to_be_deleted_.clear();

    // init newly created
    std::time_t init_time = timer_->getTime();
    BOOST_FOREACH(Timer& timer, newly_created_timers_){
        get<TE::LASTTIME>(timer) = init_time;
    }

    timers_.insert(timers_.end(),
                   newly_created_timers_.begin(),
                   newly_created_timers_.end());
    newly_created_timers_.clear();
}
