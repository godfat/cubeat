
//adapted from http://www.michaelzeilfelder.de/irrlicht/timer.h

#ifndef _SHOOTING_CUBES_UTILS_TIMER_
#define _SHOOTING_CUBES_UTILS_TIMER_

namespace irr{
class ITimer;
}

namespace psc{
namespace utils{
class Timer
{
public:
	Timer(irr::ITimer* t, bool run_on_start=false);	// startRunning_ decides if timer is running or stopped after creation
	virtual ~Timer();

  // in milliseconds
	virtual unsigned int getTime() const;
	virtual void setTime(unsigned int t);

	virtual unsigned int getLastTick() const;
	virtual double       getLastTickBySecond() const;

  // unlike irr::ITimer there's no reference counting for start/stop!
	virtual void stop();
	virtual void start();
	virtual bool isStopped() const;

	virtual unsigned int getLastStartTime() const;

	virtual void   setSpeed(double s = 1.0f);
	virtual double getSpeed() const;

	// Must be called regularly.
	virtual void tick();

private:
    irr::ITimer* irr_timer_;

    unsigned int time_;
    double       time_rest_;
    unsigned int last_real_time_;
    unsigned int last_start_time_;
    double       speed_;
    unsigned int last_tick_;
    bool         running_;
};
}//utils
}//psc

#endif // _SHOOTING_CUBES_UTILS_TIMER_
