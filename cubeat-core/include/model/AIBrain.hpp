#ifndef _SHOOTING_CUBES_MODEL_ARTIFICIAL_INTELLIGENCE_BRAIN_
#define _SHOOTING_CUBES_MODEL_ARTIFICIAL_INTELLIGENCE_BRAIN_

#include <tr1/memory>
#include <deque>
#include <vector>
#include <utility>
#include <boost/thread/mutex.hpp>

namespace psc {

namespace ctrl {
class AIPlayer;
typedef std::tr1::shared_ptr<AIPlayer> pAIPlayer;
typedef std::tr1::weak_ptr<AIPlayer> wpAIPlayer;
}

namespace model {

class SimpleMap;
typedef std::tr1::shared_ptr<SimpleMap> pSimpleMap;

class Map;
typedef std::tr1::shared_ptr<Map> pMap;
typedef std::tr1::weak_ptr<Map>  wpMap;

class AIBrain
{
public:
    typedef std::tr1::shared_ptr<AIBrain>               pointer_type;
    typedef std::tr1::shared_ptr< std::pair<int, int> > pPosition;
    typedef std::deque< pPosition >                     PositionList;

    AIBrain(ctrl::pAIPlayer const&);

    void think(std::vector<model::pSimpleMap> const& map_list,
               std::list<int> const& ally_ids,
               std::list<int> const& enemy_ids);

    bool      isThinking() const { return is_thinking_; }
    bool      needThinking();
    pPosition getCurrentCmd();
    void      popCmdQueue();

    boost::mutex& getMutex() { return cmd_queue_mutex_; }

private:
    ctrl::wpAIPlayer owner_;
    std::vector<model::pSimpleMap> map_list_;
    bool             is_thinking_;
    PositionList     shooting_pos_queue_;
    boost::mutex     cmd_queue_mutex_;
};

typedef AIBrain::pointer_type pAIBrain;

} //model
} //psc

#endif //_SHOOTING_CUBES_CTRL_ARTIFICIAL_INTELLIGENCE_BRAIN_
