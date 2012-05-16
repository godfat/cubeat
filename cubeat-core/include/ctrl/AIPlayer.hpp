#ifndef _SHOOTING_CUBES_CTRL_ARTIFICIAL_INTELLIGENCE_PLAYER_
#define _SHOOTING_CUBES_CTRL_ARTIFICIAL_INTELLIGENCE_PLAYER_

// responsible for AI to Input calling (like button state manipulation),
// and reacting to map data

#include "Player.hpp"
#include <deque>
#include <vector>
#include <boost/tr1/memory.hpp>
#include <boost/thread/thread.hpp>
#include <utility>

//for lua scripting, and this has to be global.. not in any namespaces
struct lua_State;

namespace psc {

namespace model {
class AIBrain;
class AICommand;
typedef std::tr1::shared_ptr<AICommand> pAICommand;

class SimpleMap;
typedef std::tr1::shared_ptr<SimpleMap> pSimpleMap;
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
    typedef std::deque< model::pAICommand >  CommandQueue;

    static pointer_type create(Input* input, int const& id, std::string const& ai_name = "ai/hard.lua") {
        return pointer_type(new AIPlayer(input, id, ai_name))->init();
    }

    virtual void setMapList(std::vector<presenter::wpMap> const& mlist);
    virtual void cycle();
    virtual bool startThinking();
    virtual void stopAllActions();

    //scripting usage only -- does it have to be public?
    int        cmdQueueSize();
    void       pushCommand(model::pAICommand const& cmd);
    model::pSimpleMap getAllyMap (size_t const& index);
    model::pSimpleMap getEnemyMap(size_t const& index);
    //end of scripting usage

    virtual ~AIPlayer();

protected:
    AIPlayer(Input* input, int const&, std::string const&);
    pointer_type init();

    void open_thread_to_think();
    void issue_command(model::pAICommand const&);
    void shoot(int, int, bool const& self = true);
    void haste(int);
    void hold_button(bool&, int);
    void press_button(bool&);
    void release_button(bool&);
    pointer_type self() { return self_.lock(); }

    //you can only call this after setMapList is called
    data::pViewSetting view_setting(bool const& self = true) const;

    void              think();
    void              update_map_models();
    model::pAICommand getCurrentCmd();
    void              popCmdQueue();

protected:
    wpointer_type   self_;
    model::AIBrain* brain_;
    pThread         think_thread_;
    pDummy          think_timer_;
    bool            is_executing_;
    bool            trig1_, trig2_;
    std::string     ai_name_;
    int             think_interval_;
    int             missrate_;

    std::vector<model::pSimpleMap> ally_maps_;
    std::vector<model::pSimpleMap> enemy_maps_;

    CommandQueue     cmd_queue_;

    lua_State*       L_;
};

typedef AIPlayer::pointer_type  pAIPlayer;
typedef AIPlayer::wpointer_type wpAIPlayer;

}//ctrl
}//psc

#endif //_SHOOTING_CUBES_CTRL_ARTIFICIAL_INTELLIGENCE_PLAYER_

