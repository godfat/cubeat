
#include "ctrl/AIPlayer.hpp"
#include "model/AIBrain.hpp"
#include "model/AICommand.hpp"
#include "model/SimpleMap.hpp"
#include "model/SimpleCube.hpp"
#include "model/detail/AIUtils.hpp"
#include "utils/Random.hpp"
#include "utils/Logger.hpp"
#include "script/lua_utility.hpp"
#include "Conf.hpp"

#include <boost/foreach.hpp>
#include <algorithm>
#include <iostream>

using namespace psc;
using namespace model;
using ai_detail::AIUtils;
using utils::Logger;

AIBrain::AIBrain(ctrl::pAIPlayer const& owner)
    :owner_(owner), is_thinking_(false), attack_power_(9999)
{
    std::cout << "AI processing unit created." << std::endl;
    L_ = luaL_newstate();
    luaL_openlibs(L_);
    script::Lua::run_script(L_, Conf::i().script_path("ai/easy.lua").c_str());
}

AIBrain::~AIBrain()
{
    lua_close(L_);
}

bool AIBrain::needThinking()
{
    boost::mutex::scoped_lock lock( cmd_queue_mutex_ );
    return cmd_queue_.empty();
}

void AIBrain::think(std::vector<model::pSimpleMap> ally_maps,
                    std::vector<model::pSimpleMap> enemy_maps)
{
    using namespace script;
    is_thinking_ = true;

    //since we only have two map, one for each side, so let the first in ally-list be one's self.

    ally_maps_ = ally_maps;
    enemy_maps_= enemy_maps;

    Lua::call(L_, "ai_entry", static_cast<void*>(this));

//            std::vector<pSimpleCube> garbages = AIUtils::find_garbages(self_map);
//            std::random_shuffle(garbages.begin(), garbages.end());
//
//            if( cmd_queue_.empty() ) {
//                BOOST_FOREACH(pSimpleCube& c, garbages) {
//                    pAICommand cmd = AICommand::create();
//                    cmd->delay(200).weight(1).normal_shot(c->x(), c->y());
//                    for( int i = 0; i < c->hp(); ++i )
//                        cmd_queue_.push_back( cmd );
//                    break; // only allow 1 target at a time for now
//                }
//            }
//

//                        if( ctrl::pPlayer p = owner_.lock() ) {
//                            if( p->heat() < 0.66 ) {
//                                ....
//                            }
//                        }
    is_thinking_ = false;
}

pAICommand AIBrain::getCurrentCmd()
{
    boost::mutex::scoped_lock lock( cmd_queue_mutex_ ); //necessary on reading?
    if( !cmd_queue_.empty() ) {
        return cmd_queue_.front();
    }
    else
        return pAICommand();
}

void AIBrain::popCmdQueue()
{
    boost::mutex::scoped_lock lock( cmd_queue_mutex_ );
    if( !cmd_queue_.empty() ) {
        cmd_queue_.pop_front();
    }
}

//scripting usage only
int AIBrain::cmdQueueSize() {
    boost::mutex::scoped_lock lock( cmd_queue_mutex_ ); //necessary on reading?
    return cmd_queue_.size();
}

void AIBrain::pushCommand(pAICommand cmd) {
    boost::mutex::scoped_lock lock( cmd_queue_mutex_ );
    cmd_queue_.push_back( cmd );
}

pSimpleMap AIBrain::getAllyMap (size_t const& index) {
    return ( index < ally_maps_.size() ) ? ally_maps_[index] : pSimpleMap();
}

pSimpleMap AIBrain::getEnemyMap(size_t const& index) {
    return ( index < enemy_maps_.size() ) ? enemy_maps_[index] : pSimpleMap();
}

//end of scripting usage
