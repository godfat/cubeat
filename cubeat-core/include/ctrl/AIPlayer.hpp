#ifndef _SHOOTING_CUBES_CTRL_ARTIFICIAL_INTELLIGENCE_PLAYER_
#define _SHOOTING_CUBES_CTRL_ARTIFICIAL_INTELLIGENCE_PLAYER_

// responsible for AI to Input calling (like button state manipulation),
// and reacting to map data

#include "Player.hpp"
#include <list>
#include <tr1/memory>
#include <boost/thread/thread.hpp>

namespace psc {

namespace model {
class AIBrain;
}

namespace ctrl {

class AIPlayer : public Player
{
public:
    typedef std::tr1::shared_ptr< AIPlayer > pointer_type;
    typedef std::tr1::weak_ptr  < AIPlayer > wpointer_type;
    typedef std::tr1::shared_ptr< boost::thread > pThread;
    typedef std::tr1::shared_ptr< int >      pDummy;

    static pointer_type create(Input* input = 0,
                               std::list<int> ally_ids = std::list<int>(),
                               std::list<int> enemy_ids = std::list<int>()) {
        return pointer_type(new AIPlayer(input, ally_ids, enemy_ids))->init();
    }

    void cycle();
    bool startThinking();
    void stopThinking();
    void shoot(int, int);
    void think();

    virtual ~AIPlayer();

protected:
    AIPlayer(Input* input, std::list<int> const&, std::list<int> const&);
    pointer_type init();

protected:
    model::AIBrain* brain_;
    int             think_interval_;
    pThread         think_thread_;
    pDummy          think_timer_;
};

typedef AIPlayer::pointer_type  pAIPlayer;
typedef AIPlayer::wpointer_type wpAIPlayer;

}//ctrl
}//psc

#endif //_SHOOTING_CUBES_CTRL_ARTIFICIAL_INTELLIGENCE_PLAYER_

