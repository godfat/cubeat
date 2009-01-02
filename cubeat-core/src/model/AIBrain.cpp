
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
    std::cout << "AI is thinking..." << std::endl;

    for( int i = 0; i < 200000000; i++ ) {
        data_ += 1;
        if( i % 20000000 == 0 )
            std::cout << "calculating..." << "data: " << data_ << std::endl;
        if( i % 50000000 == 0 ) {
            std::cout << "shooting..." << std::endl;
            owner_.lock()->shoot(utils::random(6), utils::random(10));
        }
    }

    std::cout << "AI thinking is done." << std::endl;
    is_thinking_ = false;

    if( data_ >= 1000000000 )
        owner_.lock()->stopThinking();
}
