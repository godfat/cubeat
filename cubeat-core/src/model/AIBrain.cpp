
#include "ctrl/AIPlayer.hpp"
#include "model/AIBrain.hpp"
#include "model/AICommand.hpp"
#include "model/SimpleMap.hpp"
#include "model/SimpleCube.hpp"
#include "model/detail/AIUtils.hpp"
#include "utils/Random.hpp"
#include "utils/Logger.hpp"
#include <boost/foreach.hpp>
#include <algorithm>
#include <iostream>

using namespace psc;
using namespace model;
using ai_detail::AIUtils;
using utils::Logger;

AIBrain::AIBrain(ctrl::pAIPlayer const& owner)
    :owner_(owner), is_thinking_(false)
{
    std::cout << "AI processing unit created." << std::endl;
}

bool AIBrain::needThinking()
{
    boost::mutex::scoped_lock lock( cmd_queue_mutex_ );
    return cmd_queue_.empty();
}

void AIBrain::think(std::vector<model::pSimpleMap> const& map_list,
                    std::list<int> const& ally_ids,
                    std::list<int> const& enemy_ids)
{
    //Logger::i().buf("brain ").buf(this).buf(" before thinking block.").endl();
    is_thinking_ = true;

    int self_index = ally_ids.front();
    //since we only have two map, one for each side, so let the first in ally-list be one's self.

    map_list_ = map_list;
    pSimpleMap self_map = map_list_[self_index]->clone();
    //Logger::i().buf("self_map: ").buf(self_map).buf(", use_count: ").buf(self_map.use_count()).endl();
    //Logger::i().buf("brain ").buf(this).buf(" checkpoint 1.").endl();
    {
        boost::mutex::scoped_lock lock( cmd_queue_mutex_ );

        //Logger::i().buf("brain ").buf(this).buf(" checkpoint 2.").endl();
        if( pSimpleCube c = AIUtils::find_keycube_for_highest_chain_power(self_map, 4) ) {
            //Logger::i().buf("brain ").buf(this).buf(" checkpoint 3a.").endl();
            pAICommand cmd = AICommand::create();
            cmd->delay(200).weight(1).normal_shot(c->x(), c->y());
            cmd_queue_.push_back( cmd );
            if( c->is_broken() )
                cmd_queue_.push_back( cmd );
            if( c->is_garbage() ) {
                cmd_queue_.push_back( cmd );
                cmd_queue_.push_back( cmd );
            }
            //Logger::i().buf("brain ").buf(this).buf(" checkpoint 4a.").endl();
        }
        else {
            //Logger::i().buf("brain ").buf(this).buf(" checkpoint 3b.").endl();
            std::vector<pSimpleCube> garbages = AIUtils::find_garbages(self_map);
            std::vector<pSimpleCube> brokens  = AIUtils::find_brokens(self_map);
            int high_col_threshold = 7;
            std::vector<int> high_cols = AIUtils::find_high_column_indexes(self_map, high_col_threshold);
            std::random_shuffle(high_cols.begin(), high_cols.end());
            std::random_shuffle(garbages.begin(), garbages.end());
            std::random_shuffle(brokens.begin(), brokens.end());

            //Logger::i().buf("brain ").buf(this).buf(" checkpoint 4b.").endl();
            if( cmd_queue_.empty() ) {
                BOOST_FOREACH(int& x, high_cols) {
                    pAICommand cmd = AICommand::create();
                    cmd->delay(200).weight(1).normal_shot(x, utils::random( high_col_threshold ) );
                    cmd_queue_.push_back( cmd );
                }
            }

            if( cmd_queue_.empty() ) {
                BOOST_FOREACH(pSimpleCube& c, brokens) {
                    pAICommand cmd = AICommand::create();
                    cmd->delay(200).weight(1).normal_shot(c->x(), c->y());
                    cmd_queue_.push_back( cmd );
                }
            }

            if( cmd_queue_.empty() ) {
                BOOST_FOREACH(pSimpleCube& c, garbages) {
                    pAICommand cmd = AICommand::create();
                    cmd->delay(200).weight(1).normal_shot(c->x(), c->y());
                    for( int i = 0; i < c->hp(); ++i )
                        cmd_queue_.push_back( cmd );
                }
            }

            //Logger::i().buf("brain ").buf(this).buf(" checkpoint 5b.").endl();
            if( cmd_queue_.empty() ) {
                if( AIUtils::grounded_cube_count(self_map) >= 36 ) {
                    int x, y;
                    do {
                        x = utils::random(self_map->ms()->width());
                        y = utils::random(self_map->ms()->height());
                    } while( !AIUtils::lookup_for_grounded(self_map, x, y) );

                    pAICommand cmd = AICommand::create();
                    cmd->delay(200).weight(1).normal_shot(x, y);
                    cmd_queue_.push_back( cmd );
                }
                else if( self_map->garbage_left() == 0 ) {
                    pAICommand cmd = AICommand::create();
                    cmd->press_trig2(); //haste here
                    cmd_queue_.push_back( cmd );
                }
            }
            //Logger::i().buf("brain ").buf(this).buf(" checkpoint 6b.").endl();
        }
    }
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
