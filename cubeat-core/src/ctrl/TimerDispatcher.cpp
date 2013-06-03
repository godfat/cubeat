
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

TimerDispatcher::TimerDispatcher(std::string const& name): name_(name) {}
TimerDispatcherNormal::TimerDispatcherNormal(std::string const& name): TimerDispatcher(name) {}
TimerDispatcherRestorable::TimerDispatcherRestorable(std::string const& name): TimerDispatcher(name) {
    data_ = new RollbackBarrierData;
    data_->frame_counter_ = 0;
    data_->old_frame_counter_ = -1;
}

TimerDispatcher::~TimerDispatcher() { delete timer_; }
TimerDispatcherNormal::~TimerDispatcherNormal() { printf("TimerDispatcherNormal died: %s\n", name_.c_str()); }
TimerDispatcherRestorable::~TimerDispatcherRestorable() {
    delete data_;
    printf("TimerDispatcherRestorable died: %s\n", name_.c_str());
}

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
    printf("TimerDispatcherRestorable: Timer(d=%d, l=%d) registered to newly_created_timers_.\n", duration, loop);
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
TimerDispatcherNormal& TimerDispatcherNormal::clear_timer_event() { return *this; }
TimerDispatcherRestorable& TimerDispatcherRestorable::clear_timer_event() { return *this; }

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

TimerDispatcherNormal& TimerDispatcherNormal::set_frame(int const& frame) {
    printf("TimerDispatcherNormal: set_frame called, should not happen.\n");
    exit(1);
    return *this;
}

TimerDispatcherRestorable& TimerDispatcherRestorable::set_frame(int const& frame) {
    data_->frame_counter_ = frame;
    return *this;
}

TimerDispatcherNormal& TimerDispatcherNormal::remove_all_newly_created_timers() {
    newly_created_timers_.clear();
    return *this;
}

TimerDispatcherRestorable& TimerDispatcherRestorable::remove_all_newly_created_timers() {
    newly_created_timers_.clear();
    printf("TimerDispatcherRestorable: Cleared newly_created_timers_.\n");
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

    if( data_->frame_counter_ == data_->old_frame_counter_ + 1 ) { // corrected frame
        timer_->tick();
        data_->recorded_times_.push_back( timer_->getTime() );
        if( data_->recorded_times_.size() > 60 ) {
            data_->recorded_times_.pop_front();
        }
        data_->old_frame_counter_ = data_->frame_counter_; // is it ok to refresh old_frame_counter here? any exceptions?
    } else {
        int frame_of_rb = data_->old_frame_counter_ - data_->frame_counter_ + 1;
        if( frame_of_rb <= static_cast<int>(data_->recorded_times_.size()) ) {
            time_t const& t = data_->recorded_times_[ data_->recorded_times_.size() - frame_of_rb ];
            timer_->setTime(t);
        }
        else {
            printf("Reversible Timer: time records underflow.\n");
            exit(1);
        }
    }
}

/// This is the Main Loop of Timer

void TimerDispatcherNormal::dispatch()
{
    init_newly_created_timer();

    TimerRemoval timers_to_be_deleted;

    std::time_t now = timer_->getTime();
    for(TimerList::iterator t = timers_.begin(), tend = timers_.end(); t != tend; ++t) {

                //extreme debug:
        printf(" duration: %ld, lasttime: %ld, loop: %d\n", get<TimerDispatcher::DURATION>(*t),
                                                            get<TimerDispatcher::LASTTIME>(*t),
                                                            get<TimerDispatcher::LOOP>(*t) );

        if( get<TimerDispatcher::CALLEE>(*t).lock() ) {
            if( now - get<TimerDispatcher::LASTTIME>(*t) >= get<TimerDispatcher::DURATION>(*t) ) {
                get<TimerDispatcher::TIMER_CB>(*t)();
                get<TimerDispatcher::LASTTIME>(*t) = now; //get<TE::LASTTIME>(*t) += get<TE::DURATION>(*t);
                int& looptimes = get<TimerDispatcher::LOOP>(*t);
                if( looptimes == 0 ) {
                    timers_to_be_deleted.push_back(t);
                } else if( looptimes > 0 ) looptimes -= 1;
            }
        }
        else timers_to_be_deleted.push_back(t);
    }

    BOOST_FOREACH(TimerList::iterator t, timers_to_be_deleted) {
        timers_.erase(t);
    }
}

void TimerDispatcherRestorable::dispatch()
{
    init_newly_created_timer();
    printf("Before dispatch (Restorable) TimerList: %d\n", timers_.size());

    TimerRemoval timers_to_be_deleted;

    std::time_t now = timer_->getTime();
    for(RestorableTimerList::iterator t = timers_.begin(), tend = timers_.end(); t != tend; ++t) {

        //extreme debug:
        printf(" duration: %ld, lasttime: %ld, loop: %d\n", get<TimerDispatcher::DURATION>(*t),
                    get<TimerDispatcher::LASTTIME>(*t), get<TimerDispatcher::LOOP>(*t) );

        if( get<TimerDispatcher::CALLEE>(*t).lock() ) {

            if( now - get<TimerDispatcher::LASTTIME>(*t) >= get<TimerDispatcher::DURATION>(*t) ) {
                get<TimerDispatcher::TIMER_CB>(*t)();
                get<TimerDispatcher::LASTTIME>(*t) = now;
                int& looptimes = get<TimerDispatcher::LOOP>(*t);
                if( looptimes == 0 ) {
                    timers_to_be_deleted.push_back(t);
                } else if( looptimes > 0 ) looptimes -= 1;
            }
        }
        else timers_to_be_deleted.push_back(t);
    }

    printf(" Blah: Before erase inactive timers (Restorable) \n");

    BOOST_FOREACH(RestorableTimerList::iterator t, timers_to_be_deleted) {
        timers_.erase(t);
    }

    printf(" Blah: End of Dispatch.\n");
}

/// Cleanup methods of Timer

void TimerDispatcherNormal::init_newly_created_timer()
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

/// UPDATE NOTE (see purple texts in test_ggpo too):
/// All un-synchronized part of TimerDispatcher when I was inspecting with system("pause") seemed resolved.
/// And always remember the metadata of std containers, such as begin()/end()/size() etc
/// reside in the enveloping class "physically".

/// Current solution note: timers_ and newly_created_timers_ have to be Restorable
/// (since newly_created_timers_ goes across frames), but frame_counter_ & game-time related
/// variables should be kept from Restorable with a barrier, because those are "INPUT" data,
/// not game states. Review this if in doubt => game_state[f+1] = update(game_state[f], input{key, time, random})

void TimerDispatcherRestorable::init_newly_created_timer()
{
    printf("TimerDispatcherRestorable::init_newly_created_timer: size: %d, %x, %x\n",
           newly_created_timers_.size(),
           newly_created_timers_.begin(),
           newly_created_timers_.end());

    std::time_t init_time = timer_->getTime();
    BOOST_FOREACH(Timer& timer, newly_created_timers_){
        get<TimerDispatcher::LASTTIME>(timer) = init_time;
        printf(" -- Debug: callee: %x, lasttime: %ld, duration: %ld, loop: %d\n",
               get<TimerDispatcher::CALLEE>(timer).lock().get(),
               get<TimerDispatcher::LASTTIME>(timer),
               get<TimerDispatcher::DURATION>(timer),
               get<TimerDispatcher::LOOP>(timer));
    }
    printf("Before insert newly_created_timers (Restorable) TimerList: %d\n", timers_.size());
    timers_.insert(timers_.end(),
                   newly_created_timers_.begin(),
                   newly_created_timers_.end());
    newly_created_timers_.clear();
}
