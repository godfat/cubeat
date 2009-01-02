
#include "ctrl/AIPlayer.hpp"
#include "model/AIBrain.hpp"
#include "EventDispatcher.hpp"
#include "Input.hpp"

using namespace psc;
using namespace ctrl;
using std::tr1::bind;

AIPlayer::AIPlayer(Input* input, std::list<int> const& ally_ids, std::list<int> const& enemy_ids)
    :Player(input, ally_ids, enemy_ids), brain_(0), think_interval_(500)
{
}

AIPlayer::~AIPlayer()
{
    delete brain_;
}

pAIPlayer AIPlayer::init()
{
    Player::init();
    pAIPlayer self = std::tr1::static_pointer_cast<AIPlayer>(shared_from_this());
    brain_ = new model::AIBrain(self);

    return self;
}

void AIPlayer::think()
{
    if( !brain_->isThinking() )
        think_thread_ = pThread(new boost::thread( bind( &model::AIBrain::think, brain_)));
}

bool AIPlayer::startThinking()
{
    if( brain_ && !brain_->isThinking() ) {
        std::cout << "CPU AI started thinking ..." << std::endl;
        think_timer_ = pDummy(new int);
        EventDispatcher::i().subscribe_timer( bind( &AIPlayer::think, this), think_timer_, think_interval_, -1);
        return true;
    }
    else return false;
}

void AIPlayer::stopThinking()
{
    if( think_timer_ && think_thread_ ) {
        std::cout << "AIPlayer stopped thinking." << std::endl;
        think_timer_.reset();
        think_thread_->join();
    }
}

void AIPlayer::shoot(int x, int y) //we must know ViewSettings here.
{
    std::cout << " .. call shooting: " << x << ", " << y << std::endl;
    //manipulate input and cursor here
}

void AIPlayer::cycle()
{
}
