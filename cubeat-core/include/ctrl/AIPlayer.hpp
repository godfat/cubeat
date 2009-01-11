#ifndef _SHOOTING_CUBES_CTRL_ARTIFICIAL_INTELLIGENCE_PLAYER_
#define _SHOOTING_CUBES_CTRL_ARTIFICIAL_INTELLIGENCE_PLAYER_

// responsible for AI to Input calling (like button state manipulation),
// and reacting to map data

#include "Player.hpp"
#include <list>
#include <tr1/memory>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <utility>

namespace psc {

namespace model {
class AIBrain;
}

namespace ctrl {

class Button;

class AIPlayer : public Player
{
public:
    typedef std::tr1::shared_ptr< AIPlayer > pointer_type;
    typedef std::tr1::weak_ptr  < AIPlayer > wpointer_type;
    typedef std::tr1::shared_ptr< boost::thread > pThread;
    typedef std::tr1::shared_ptr< std::pair<int, int> > pPosition;
    typedef std::tr1::shared_ptr< int >      pDummy;

    static pointer_type create(Input* input,
                               data::pViewSetting const& view_setting) {
        return pointer_type(new AIPlayer(input, view_setting))->init();
    }

    void cycle();
    bool startThinking();
    void stopThinking();
    void shoot(int, int);
    void think();

    pPosition probing_brain_data();
    boost::mutex& getMutex() { return think_mutex_; }

    virtual ~AIPlayer();

protected:
    AIPlayer(Input* input, data::pViewSetting const&);
    pointer_type init();
    void hold_button(ctrl::Button&, int);
    void press_button(ctrl::Button&);
    void release_button(ctrl::Button&);

protected:
    model::AIBrain* brain_;
    int             think_interval_;
    pThread         think_thread_;
    pDummy          think_timer_;
    boost::mutex    think_mutex_;
};

typedef AIPlayer::pointer_type  pAIPlayer;
typedef AIPlayer::wpointer_type wpAIPlayer;

}//ctrl
}//psc

#endif //_SHOOTING_CUBES_CTRL_ARTIFICIAL_INTELLIGENCE_PLAYER_

