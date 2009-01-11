#ifndef _SHOOTING_CUBES_MODEL_ARTIFICIAL_INTELLIGENCE_BRAIN_
#define _SHOOTING_CUBES_MODEL_ARTIFICIAL_INTELLIGENCE_BRAIN_

#include <tr1/memory>
#include <deque>
#include <utility>
#include <boost/thread/mutex.hpp>

namespace psc {

namespace ctrl {
class AIPlayer;
typedef std::tr1::shared_ptr<AIPlayer> pAIPlayer;
typedef std::tr1::weak_ptr<AIPlayer> wpAIPlayer;
}

namespace model {

class AIBrain
{
public:
    typedef std::deque< std::pair<int, int> > PositionList;
    typedef std::tr1::shared_ptr< std::pair<int, int> > pPosition;

    AIBrain(ctrl::pAIPlayer const&);
    void think();

    bool isThinking() const { return is_thinking_; }
    int  getData()    const { return data_; }
    PositionList& getPosList() { return shooting_pos_queue_; }
    pPosition     getCurrentCmd();
    void          popCmdQueue();

private:
    ctrl::wpAIPlayer owner_;
    int  data_;
    bool is_thinking_;
    PositionList shooting_pos_queue_;
    boost::mutex cmd_queue_mutex_;
};

} //model
} //psc

#endif //_SHOOTING_CUBES_CTRL_ARTIFICIAL_INTELLIGENCE_BRAIN_
