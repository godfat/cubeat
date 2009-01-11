
#include "ctrl/AIPlayer.hpp"
#include "model/AIBrain.hpp"
#include "utils/Random.hpp"

#include <iostream>

using namespace psc;
using namespace model;

AIBrain::AIBrain(ctrl::pAIPlayer const& owner)
    :owner_(owner), data_(0), is_thinking_(false)
{
    std::cout << "AI processing unit created." << std::endl;
}

void AIBrain::think()
{
    is_thinking_ = true;

    for( int i = 0; i < 200000000; i++ ) {
        data_ += 1;
        if( i % 50000000 == 0 ) {
            boost::mutex::scoped_lock( cmd_queue_mutex_ );
            shooting_pos_queue_.push_back( std::make_pair(utils::random(6), utils::random(10)) );
        }
    }

    is_thinking_ = false;

    if( data_ >= 1000000000 )
        owner_.lock()->stopThinking();
}

AIBrain::pPosition AIBrain::getCurrentCmd()
{
    boost::mutex::scoped_lock( cmd_queue_mutex_ );
    if( !shooting_pos_queue_.empty() ) {
        return pPosition( &shooting_pos_queue_.front() );
    }
    else
        return pPosition();
}

void AIBrain::popCmdQueue()
{
    boost::mutex::scoped_lock( cmd_queue_mutex_ );
    if( !shooting_pos_queue_.empty() ) {
        shooting_pos_queue_.pop_front();
    }
}
