#ifndef _SHOOTING_CUBES_CTRL_ARTIFICIAL_INTELLIGENCE_PLAYER_
#define _SHOOTING_CUBES_CTRL_ARTIFICIAL_INTELLIGENCE_PLAYER_

// responsible for AI to Input calling (like button state manipulation),
// and reacting to map data

#include "Player.hpp"
#include <list>
#include <boost/tr1/memory.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <utility>

namespace psc {

namespace model {
class AIBrain;
class AICommand;
typedef std::tr1::shared_ptr<AICommand> pAICommand;
}

namespace ctrl {

class Button;

class AIPlayer : public Player
{
public:
    typedef std::tr1::shared_ptr< AIPlayer > pointer_type;
    typedef std::tr1::weak_ptr  < AIPlayer > wpointer_type;
    typedef std::tr1::shared_ptr< boost::thread > pThread;
    typedef std::tr1::shared_ptr< int >      pDummy;

    static pointer_type create(Input* input,
                               data::pViewSetting const& view_setting) {
        return pointer_type(new AIPlayer(input, view_setting))->init();
    }

    virtual void cycle();
    virtual bool startThinking();
    virtual void stopThinking();

    boost::mutex& getMutex() { return think_mutex_; }

    virtual ~AIPlayer();

protected:
    AIPlayer(Input* input, data::pViewSetting const&);
    pointer_type init();

    void think();
    void issue_command(model::pAICommand const&);
    void shoot(int, int);
    void haste(int);
    void hold_button(bool&, int);
    void press_button(bool&);
    void release_button(bool&);
    pointer_type self() { return self_.lock(); }

protected:
    wpointer_type   self_;
    model::AIBrain* brain_;
    int             think_interval_;
    pThread         think_thread_;
    pDummy          think_timer_;
    boost::mutex    think_mutex_;
    bool            is_executing_;
    bool            trig1_, trig2_;
};

typedef AIPlayer::pointer_type  pAIPlayer;
typedef AIPlayer::wpointer_type wpAIPlayer;

}//ctrl
}//psc

#endif //_SHOOTING_CUBES_CTRL_ARTIFICIAL_INTELLIGENCE_PLAYER_

