#ifndef _SHOOTING_CUBES_CTRL_TIMER_DISPATCHER_
#define _SHOOTING_CUBES_CTRL_TIMER_DISPATCHER_

#include "utils/pool/pool_alloc.hpp"
#include <boost/tr1/memory.hpp>
#include <boost/tr1/tuple.hpp>
#include <boost/tr1/functional.hpp>
#include <ctime>
#include <string>
#include <deque>
#include <list>

namespace psc {

namespace utils {
class Timer;
}

namespace ctrl {

class EventDispatcher;

class TimerDispatcher : public std::tr1::enable_shared_from_this<TimerDispatcher>
{
public:
    typedef std::tr1::shared_ptr<TimerDispatcher> pointer_type;
    typedef std::tr1::weak_ptr<TimerDispatcher>   wpointer_type;
    typedef std::tr1::weak_ptr<void>              wpvoid;

    typedef std::tr1::function<void()>                                            TimerCallback;
    typedef std::tr1::tuple<TimerCallback, std::time_t, std::time_t, int, wpvoid> Timer;

    virtual TimerDispatcher&
    subscribe(TimerCallback const&, wpvoid const&, int const&, int const& loop = 0) = 0;
    virtual TimerDispatcher&
    subscribe(TimerCallback const&, int const&, int const& loop = 0) = 0;
    virtual TimerDispatcher& clear_timer_event(); //note: has bug, don't use.
    //note: call to this has no effect, don't sure why. need to check in the future.
    virtual ~TimerDispatcher();

    TimerDispatcher& start();
    TimerDispatcher& stop();
    TimerDispatcher& set_speed(double const&);
    TimerDispatcher& set_time(time_t const&);
    TimerDispatcher& set_frame(int const&);
    bool             is_stopped() const;
    std::time_t      get_time()   const;
    std::string      get_name()   const { return name_; }
    double           get_speed()  const;

protected:
    enum { TIMER_CB, DURATION, LASTTIME, LOOP, CALLEE };

    TimerDispatcher(std::string const& name);
    TimerDispatcher(TimerDispatcher const&); //copy constructor, not-implemented.
    void init();

    // This will be called by EventDispatcher through friend
    virtual void dispatch() = 0;

    // But this won't.
    virtual void cleanup_timer_and_init_newly_created_timer() = 0;
    virtual void tick() = 0;

    std::deque< time_t > recorded_times_;

    int           frame_counter_, old_frame_counter_;
    std::string   name_;
    utils::Timer* timer_;

    friend class EventDispatcher; //for EventDispatcher to call dispatch & tick
};

class TimerDispatcherNormal : public TimerDispatcher
{
public:
    typedef std::tr1::shared_ptr<TimerDispatcherNormal> pointer_type;
    typedef std::tr1::weak_ptr<TimerDispatcherNormal>   wpointer_type;
    typedef std::tr1::weak_ptr<void>                    wpvoid;

    typedef std::list<TimerDispatcher::Timer>           TimerList;
    typedef std::list<TimerList::iterator>              TimerRemoval;

    static pointer_type create(std::string const& name) {
        return pointer_type(new TimerDispatcherNormal(name))->init();
    }

    virtual TimerDispatcherNormal&
    subscribe(TimerDispatcher::TimerCallback const&, wpvoid const&, int const&, int const& loop = 0);
    virtual TimerDispatcherNormal&
    subscribe(TimerDispatcher::TimerCallback const&, int const&, int const& loop = 0);
    virtual TimerDispatcherNormal& clear_timer_event(); //note: remain unimplemented because of the unresolved bug.
    virtual ~TimerDispatcherNormal();

protected:
    TimerDispatcherNormal(std::string const& name);
    TimerDispatcherNormal(TimerDispatcherNormal const&); //copy constructor, not-implemented.
    pointer_type init();

    virtual void dispatch();
    virtual void cleanup_timer_and_init_newly_created_timer();
    virtual void tick();

    TimerList     timers_, newly_created_timers_;
    TimerRemoval  timers_to_be_deleted_;

    wpointer_type self_;
};

class TimerDispatcherRestorable : public TimerDispatcher
{
public:
    typedef std::tr1::shared_ptr<TimerDispatcherRestorable> pointer_type;
    typedef std::tr1::weak_ptr<TimerDispatcherRestorable>   wpointer_type;
    typedef std::tr1::weak_ptr<void>                        wpvoid;

    typedef std::list<TimerDispatcher::Timer,
                      boost::fast_pool_allocator<TimerDispatcher::Timer> >  TimerList;
    typedef std::list<TimerList::iterator,
                      boost::fast_pool_allocator<TimerList::iterator> >     TimerRemoval;

    static pointer_type create(std::string const& name) {
        return pointer_type(new TimerDispatcherRestorable(name))->init();
    }

    virtual TimerDispatcherRestorable&
    subscribe(TimerDispatcher::TimerCallback const&, wpvoid const&, int const&, int const& loop = 0);
    virtual TimerDispatcherRestorable&
    subscribe(TimerDispatcher::TimerCallback const&, int const&, int const& loop = 0);
    virtual TimerDispatcherRestorable& clear_timer_event(); //note: remain unimplemented because of the unresolved bug.
    virtual ~TimerDispatcherRestorable();

protected:
    TimerDispatcherRestorable(std::string const& name);
    TimerDispatcherRestorable(TimerDispatcherRestorable const&); //copy constructor, not-implemented.
    pointer_type init();

    virtual void dispatch();
    virtual void cleanup_timer_and_init_newly_created_timer();
    virtual void tick();

    TimerList     timers_, newly_created_timers_;
    TimerRemoval  timers_to_be_deleted_;

    wpointer_type self_;
};

typedef TimerDispatcher::pointer_type  pTimerDispatcher;
typedef TimerDispatcher::wpointer_type wpTimerDispatcher;

} //ctrl
} //psc

#endif

