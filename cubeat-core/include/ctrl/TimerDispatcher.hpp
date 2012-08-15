#ifndef _SHOOTING_CUBES_CTRL_TIMER_DISPATCHER_
#define _SHOOTING_CUBES_CTRL_TIMER_DISPATCHER_

#include <boost/tr1/memory.hpp>
#include <boost/tr1/tuple.hpp>
#include <boost/tr1/functional.hpp>
#include <ctime>
#include <string>
#include <list>

namespace psc {

namespace utils {
class Timer;
}

namespace ctrl {

class EventDispatcher;

class TimerDispatcher : public std::tr1::enable_shared_from_this<TimerDispatcher>
{
    typedef std::tr1::shared_ptr<void>                                     pvoid;
    typedef std::tr1::weak_ptr<void>                                       wpvoid;

    typedef std::tr1::function<void()>                                     TimerCallback;
    typedef std::tr1::tuple<TimerCallback, std::time_t, std::time_t, int, wpvoid> Timer;
    typedef std::list<Timer>                                               TimerList;
    typedef std::list< TimerList::iterator >                               TimerRemoval;

public:
    typedef std::tr1::shared_ptr<TimerDispatcher> pointer_type;
    typedef std::tr1::weak_ptr<TimerDispatcher>   wpointer_type;

    static pointer_type create(std::string const& name) {
        return pointer_type(new TimerDispatcher(name))->init();
    }

    TimerDispatcher&
    subscribe(TimerCallback const&, wpvoid const&, int const&, int loop = 0);
    TimerDispatcher&
    subscribe(TimerCallback const&, int const&, int loop = 0);

    TimerDispatcher& start();
    TimerDispatcher& stop();
    TimerDispatcher& set_speed(double const&);
    bool             is_stopped() const;
    std::time_t      get_time()   const;
    std::string      get_name()   const { return name_; }
    double           get_speed()  const;

    TimerDispatcher& clear_timer_event(); //note: has bug, don't use.
    //note: call to this has no effect, don't sure why. need to check in the future.
    ~TimerDispatcher();

private:
    struct TE{enum{TIMER_CB, DURATION, LASTTIME, LOOP, CALLEE};};
    enum { TIMER_TIME, TIMER };

    TimerDispatcher(std::string const& name);
    TimerDispatcher(TimerDispatcher const&); //copy constructor, not-implemented.
    pointer_type init();

    void dispatch();
    void cleanup_timer_and_init_newly_created_timer();
    void tick();

    TimerList     timers_, newly_created_timers_;
    TimerRemoval  timers_to_be_deleted_;
    std::string   name_;
    utils::Timer* timer_;

    wpointer_type self_;

    friend class EventDispatcher; //for EventDispatcher to call dispatch & tick
};

typedef TimerDispatcher::pointer_type  pTimerDispatcher;
typedef TimerDispatcher::wpointer_type wpTimerDispatcher;

} //ctrl
} //psc

#endif

