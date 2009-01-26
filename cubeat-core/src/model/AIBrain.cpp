
#include "ctrl/AIPlayer.hpp"
#include "model/AIBrain.hpp"
#include "model/SimpleMap.hpp"
#include "model/Map.hpp"
#include "model/detail/AIUtils.hpp"
#include "utils/Random.hpp"

#include <iostream>

using namespace psc;
using namespace model;
using ai_detail::AIUtils;

AIBrain::AIBrain(ctrl::pAIPlayer const& owner)
    :owner_(owner), is_thinking_(false)
{
    std::cout << "AI processing unit created." << std::endl;
}

bool AIBrain::needThinking()
{
    boost::mutex::scoped_lock lock( cmd_queue_mutex_ );
    return shooting_pos_queue_.size() < 2;
}

void AIBrain::think(std::vector<model::pSimpleMap> const& map_list,
                    std::list<int> const& ally_ids,
                    std::list<int> const& enemy_ids)
{
    is_thinking_ = true;

    int self_index = ally_ids.front();
    //since we only have two map, one for each side, so let the first in ally-list be one's self.

    map_list_ = map_list;
    pSimpleMap self_map = map_list_[self_index];
    {
        boost::mutex::scoped_lock lock( cmd_queue_mutex_ );
        map_list_[0]->print_data_for_debug();
    }

    for( int x = 0; x < self_map->ms()->width(); ++x ) {
        if( AIUtils::lookup(self_map, x, 4) ) {
            boost::mutex::scoped_lock lock( cmd_queue_mutex_ );
            shooting_pos_queue_.push_back( pPosition(new std::pair<int, int>(std::make_pair(x, 4))) );
        }
    }
    is_thinking_ = false;
//  owner_.lock()->stopThinking();
}

AIBrain::pPosition AIBrain::getCurrentCmd()
{
    boost::mutex::scoped_lock lock( cmd_queue_mutex_ );
    if( !shooting_pos_queue_.empty() ) {
        return shooting_pos_queue_.front();
    }
    else
        return pPosition();
}

void AIBrain::popCmdQueue()
{
    boost::mutex::scoped_lock lock( cmd_queue_mutex_ );
    if( !shooting_pos_queue_.empty() ) {
        shooting_pos_queue_.pop_front();
    }
}
