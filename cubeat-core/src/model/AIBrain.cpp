
#include "ctrl/AIPlayer.hpp"
#include "model/AIBrain.hpp"
#include "model/AICommand.hpp"
#include "model/SimpleMap.hpp"
#include "model/SimpleCube.hpp"
#include "model/detail/AIUtils.hpp"
#include "utils/Random.hpp"
#include "utils/Logger.hpp"
#include "script/lua_utility.hpp"
#include "script/lua_AI_bindings.hpp"
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

void AIBrain::think(std::vector<model::pSimpleMap> map_list,
                    std::list<int> ally_ids,
                    std::list<int> enemy_ids)
{
    using namespace script;
    //Logger::i().buf("brain ").buf(this).buf(" before thinking block.").endl();
    is_thinking_ = true;
    int self_index = ally_ids.front();
    int enemy_index = enemy_ids.front();
    //since we only have two map, one for each side, so let the first in ally-list be one's self.

    map_list_ = map_list;
    pSimpleMap self_map = map_list_[self_index]->clone();
    pSimpleMap enemy_map = map_list_[enemy_index]->clone();
    //Logger::i().buf("self_map: ").buf(self_map).buf(", use_count: ").buf(self_map.use_count()).endl();
    //Logger::i().buf("brain ").buf(this).buf(" checkpoint 1.").endl();

    pSimpleMap *p = new pSimpleMap, *q = new pSimpleMap;
    *p = self_map;
    *q = enemy_map;
    Lua::call(L_, "ai_entry", static_cast<void*>(p), static_cast<void*>(q));
    //don't delete here! let lua gc invoke the destruction!
    Logger::i().buf("Verify ").buf(self_map).buf(" use_count: ").buf(self_map.use_count()).endl();
//    {
//        boost::mutex::scoped_lock lock( cmd_queue_mutex_ );
//
//        //Logger::i().buf("brain ").buf(this).buf(" checkpoint 2.").endl();
//        int attack_threshold = 8;
//        if( attack_power_ < 9  )      attack_threshold = 2;
//        else if( attack_power_ < 20 ) attack_threshold = 4;
//        if( self_map->warning_level() > 50 )
//            attack_threshold = 1;
//        else if( self_map->warning_level() > 25 )
//            attack_threshold = 2;
//        else if( self_map->warning_level() > 0 )
//            attack_threshold = 3;
//        else if( AIUtils::grounded_cube_count(self_map) + self_map->garbage_left() >
//                 self_map->ms()->width() * (self_map->ms()->height()-1) )
//            attack_threshold = 1;
//
//        pSimpleCube keycube = AIUtils::find_keycube_for_highest_chain_power(self_map, attack_threshold, attack_power_);
//        if( enemy_map->garbage_left() < attack_power_ * 2 && keycube ) {
//            //Logger::i().buf(this).buf(" found highest chain_power at: ").buf(c->x()).buf(", ").buf(c->y()).endl();
//            pAICommand cmd = AICommand::create();
//            cmd->delay(200).weight(1).normal_shot(keycube->x(), keycube->y());
//            cmd_queue_.push_back( cmd );
//            if( keycube->is_broken() )
//                cmd_queue_.push_back( cmd );
//            if( keycube->is_garbage() ) {
//                cmd_queue_.push_back( cmd );
//                cmd_queue_.push_back( cmd );
//            }
//            //Logger::i().buf("brain ").buf(this).buf(" checkpoint 4a.").endl();
//        }
//        else
//        {
//            //Logger::i().buf("brain ").buf(this).buf(" checkpoint 3b.").endl();
//            std::vector<pSimpleCube> garbages = AIUtils::find_garbages(self_map);
//            std::vector<pSimpleCube> brokens  = AIUtils::find_brokens(self_map);
//            int high_col_threshold = 9;
//            std::vector<int> high_cols = AIUtils::find_high_column_indexes(self_map, high_col_threshold);
//            std::random_shuffle(high_cols.begin(), high_cols.end());
//            std::random_shuffle(garbages.begin(), garbages.end());
//            std::random_shuffle(brokens.begin(), brokens.end());
//
//            //Logger::i().buf("brain ").buf(this).buf(" checkpoint 4b.").endl();
//
//            BOOST_FOREACH(pSimpleCube& c, brokens) {
//                pAICommand cmd = AICommand::create();
//                cmd->delay(200).weight(1).normal_shot(c->x(), c->y());
//                cmd_queue_.push_back( cmd );
//                break; // only allow 1 target at a time for now
//            }
//
////            if( cmd_queue_.empty() ) {
////                BOOST_FOREACH(pSimpleCube& c, garbages) {
////                    pAICommand cmd = AICommand::create();
////                    cmd->delay(200).weight(1).normal_shot(c->x(), c->y());
////                    for( int i = 0; i < c->hp(); ++i )
////                        cmd_queue_.push_back( cmd );
////                    break; // only allow 1 target at a time for now
////                }
////            }
//
//            BOOST_FOREACH(int& x, high_cols) {
//                pAICommand cmd = AICommand::create();
//                cmd->delay(200).weight(1).normal_shot(x, utils::random( high_col_threshold/2 ) );
//                cmd_queue_.push_back( cmd );
//                break; // only allow 1 target at a time for now
//            }
//
//            //Logger::i().buf("brain ").buf(this).buf(" checkpoint 5b.").endl();
//            if( cmd_queue_.empty() ) {
//                if( AIUtils::grounded_cube_count(self_map) >= 48 ) {
//                    int x, y;
//                    do {
//                        x = utils::random(self_map->ms()->width());
//                        y = utils::random(self_map->ms()->height() / 2);
//                    } while( !AIUtils::lookup_for_grounded(self_map, x, y) );
//
//                    pAICommand cmd = AICommand::create();
//                    cmd->delay(200).weight(1).normal_shot(x, y);
//                    cmd_queue_.push_back( cmd );
//                }
//                else {
//                    try { //see if we can catch anything at all here...
//                        if( ctrl::pPlayer p = owner_.lock() ) {
//                            if( p->heat() < 0.66 ) {
//                                pAICommand cmd = AICommand::create();
//                                cmd->press_trig2(); //haste here
//                                cmd_queue_.push_back( cmd );
//                            }
//                        }
//                    } catch(std::exception& e) {
//                        Logger::i().buf(e.what()).endl();
//                    }
//                }
//            }
//            //Logger::i().buf("brain ").buf(this).buf(" checkpoint 6b.").endl();
//        }
//    }
    is_thinking_ = false;
    //Logger::i().buf("brain ").buf(this).buf(" after thinking block.").endl();
//  owner_.lock()->stopThinking();
}

pAICommand AIBrain::getCurrentCmd()
{
    boost::mutex::scoped_lock lock( cmd_queue_mutex_ );
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
