
//adapted from http://www.michaelzeilfelder.de/irrlicht/timer.cpp

#include "ITimer.h" //irrlicht
#include "utils/Timer.hpp"

using namespace psc;
using namespace utils;

//start from here )#@*U(*RJEF(

Timer::Timer(irr::ITimer* t, bool run_on_start)
    : irr_timer_(t)
    , time_(0)
    , time_rest_(0)
    , last_real_time_(0)
    , last_start_time_(0)
    , speed_(1.0)
    , last_tick_(0)
    , running_(run_on_start)
{
}

Timer::~Timer()
{
}

unsigned int Timer::getTime() const
{
    return time_;
}

void Timer::setTime(unsigned int t)
{
    time_ = t;
    time_rest_ = 0;
}

unsigned int Timer::getLastTick() const
{
    return last_tick_;
}

double Timer::getLastTickBySecond() const
{
    return static_cast<double>(last_tick_) / 1000.0;
}


void Timer::stop()
{
    running_ = false;
    last_tick_ = 0;
}

void Timer::start()
{
    running_ = true;
    if ( irr_timer_ ) {
        last_real_time_ = irr_timer_->getRealTime();
    }
    last_start_time_ = time_;
}

bool Timer::isStopped() const
{
    return !running_;
}

unsigned int Timer::getLastStartTime() const
{
    return last_start_time_;
}

void Timer::setSpeed(double s)
{
    speed_ = s;
}

double Timer::getSpeed() const
{
    return speed_;
}

void Timer::tick()
{
    if ( last_real_time_ == 0 ) {
        if ( irr_timer_ ) {
            last_real_time_ = irr_timer_->getRealTime();
        }
    }

    if ( running_ ) {
        unsigned int time_now = 0;

        if ( irr_timer_ ) {
            time_now = irr_timer_->getRealTime();
            double time_add = static_cast<double>(time_now - last_real_time_) * speed_ + time_rest_;
            last_tick_ = static_cast<int>(time_add);
            time_ += last_tick_;
            time_rest_ = time_add - last_tick_;   // "Remainder" to deal with precision differences?
        }
    }
    if ( irr_timer_ ) {
        last_real_time_ = irr_timer_->getRealTime();
    }
}

