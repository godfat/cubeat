#ifndef _SHOOTING_CUBES_MODEL_ARTIFICIAL_INTELLIGENCE_BRAIN_
#define _SHOOTING_CUBES_MODEL_ARTIFICIAL_INTELLIGENCE_BRAIN_

#include <boost/tr1/memory.hpp>
#include <deque>
#include <vector>
#include <utility>
#include <boost/thread/mutex.hpp>

//for lua scripting, and this has to be global.. not in any namespaces
struct lua_State;

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

class AICommand;
typedef std::tr1::shared_ptr<AICommand> pAICommand;

class AIBrain
{
public:
    typedef std::tr1::shared_ptr<AIBrain> pointer_type;
    typedef std::deque< pAICommand >      CommandQueue;

    AIBrain(ctrl::pAIPlayer const&);

    void think(std::vector<model::pSimpleMap> map_list,
               std::list<int> ally_ids,
               std::list<int> enemy_ids);

    bool       isThinking() const { return is_thinking_; }
    bool       needThinking();
    pAICommand getCurrentCmd();
    void       popCmdQueue();

    AIBrain&   power(int const& i)       { attack_power_ = i; return *this; }
    int        power() const             { return attack_power_; }

    boost::mutex& getMutex() { return cmd_queue_mutex_; }

private:
    ctrl::wpAIPlayer owner_;
    std::vector<model::pSimpleMap> map_list_;
    bool             is_thinking_;
    int              attack_power_;
    CommandQueue     cmd_queue_;
    boost::mutex     cmd_queue_mutex_;

    lua_State*       L_;
};

typedef AIBrain::pointer_type pAIBrain;

} //model
} //psc

#endif //_SHOOTING_CUBES_CTRL_ARTIFICIAL_INTELLIGENCE_BRAIN_
