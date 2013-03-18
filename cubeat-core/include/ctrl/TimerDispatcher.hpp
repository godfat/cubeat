#ifndef _SHOOTING_CUBES_CTRL_TIMER_DISPATCHER_
#define _SHOOTING_CUBES_CTRL_TIMER_DISPATCHER_

#include "utils/pool/pool_alloc.hpp"
#include "utils/ObjectPool.hpp"
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

    virtual TimerDispatcher& remove_all_newly_created_timers() = 0;
    virtual TimerDispatcher& set_frame(int const&) = 0;

    virtual ~TimerDispatcher();

    TimerDispatcher& start();
    TimerDispatcher& stop();
    TimerDispatcher& set_speed(double const&);
    TimerDispatcher& set_time(time_t const&);
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
    virtual void init_newly_created_timer() = 0;
    virtual void tick() = 0;

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

    virtual TimerDispatcherNormal& remove_all_newly_created_timers();
    virtual TimerDispatcherNormal& set_frame(int const&);

    virtual ~TimerDispatcherNormal();

protected:
    TimerDispatcherNormal(std::string const& name);
    TimerDispatcherNormal(TimerDispatcherNormal const&); //copy constructor, not-implemented.
    pointer_type init();

    virtual void dispatch();
    virtual void init_newly_created_timer();
    virtual void tick();

    TimerList     timers_, newly_created_timers_;

    wpointer_type self_;
};

class TimerDispatcherRestorable : public TimerDispatcher
{
public:
    typedef std::tr1::shared_ptr<TimerDispatcherRestorable> pointer_type;
    typedef std::tr1::weak_ptr<TimerDispatcherRestorable>   wpointer_type;
    typedef std::tr1::weak_ptr<void>                        wpvoid;

    typedef std::list<TimerDispatcher::Timer>                               TimerList;
    typedef std::list<TimerDispatcher::Timer,
                      boost::fast_pool_allocator<TimerDispatcher::Timer> >  RestorableTimerList;
//    typedef std::list<TimerList::iterator,
//                      boost::fast_pool_allocator<TimerList::iterator> >     TimerRemoval;
    typedef std::list<RestorableTimerList::iterator>                        TimerRemoval;
    // Removal doesn't have to be rolled-back, it happens right after every dispatch

    static pointer_type create(std::string const& name) {
        //return pointer_type(new TimerDispatcherRestorable(name))->init();
        // 2013.3.18
        // It's improbable to rollback the std containers inside this class without rolling-back
        // this class itself. Key metadata reside in this class (such as where iterator begin/end point to.)
        return utils::ObjectPoolRestorable<TimerDispatcherRestorable>::create(name)->init();
    }

    TimerDispatcherRestorable(std::string const& name);
    virtual ~TimerDispatcherRestorable();

    virtual TimerDispatcherRestorable&
    subscribe(TimerDispatcher::TimerCallback const&, wpvoid const&, int const&, int const& loop = 0);
    virtual TimerDispatcherRestorable&
    subscribe(TimerDispatcher::TimerCallback const&, int const&, int const& loop = 0);
    virtual TimerDispatcherRestorable& clear_timer_event(); //note: remain unimplemented because of the unresolved bug.

    virtual TimerDispatcherRestorable& remove_all_newly_created_timers();
    virtual TimerDispatcherRestorable& set_frame(int const&);

protected:
    TimerDispatcherRestorable(TimerDispatcherRestorable const&); //copy constructor, not-implemented.
    pointer_type init();

    virtual void dispatch();
    virtual void init_newly_created_timer();
    virtual void tick();

    struct RollbackBarrierData { // It's funny, but looks like it's useful in this way, to shield something from rollback.
        std::deque< time_t > recorded_times_;
        int                  frame_counter_, old_frame_counter_;
    };

    RollbackBarrierData* data_;
    RestorableTimerList  timers_, newly_created_timers_;

    wpointer_type self_;
};

typedef TimerDispatcher::pointer_type  pTimerDispatcher;
typedef TimerDispatcher::wpointer_type wpTimerDispatcher;

} //ctrl
} //psc

#endif

