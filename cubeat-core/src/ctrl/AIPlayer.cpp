
#include "ctrl/AIPlayer.hpp"
#include "data/ViewSetting.hpp"
#include "model/AIBrain.hpp"
#include "model/AICommand.hpp"
#include "presenter/Map.hpp"
#include "view/Sprite.hpp"
#include "EventDispatcher.hpp"
#include "Input.hpp"
#include "Accessors.hpp"
#include "EasingEquations.hpp"
#include "utils/Logger.hpp"
#include "utils/Random.hpp"
#include "Conf.hpp"

#include <boost/tr1/functional.hpp>

using namespace psc;
using namespace ctrl;
using std::tr1::bind;
using std::tr1::static_pointer_cast;
using std::tr1::ref;
using std::tr1::function;
using utils::Logger;

AIPlayer::AIPlayer(Input* input, int const& id)
    :Player(input, id), brain_(0), think_interval_(250), is_executing_(false),
     trig1_(false), trig2_(false)
{
}

AIPlayer::~AIPlayer()
{
    delete brain_;
}

pAIPlayer AIPlayer::init()
{
    Player::init();
    self_ = std::tr1::static_pointer_cast<AIPlayer>(shared_from_this());
    brain_ = new model::AIBrain(self());

    int x = utils::random( Conf::i().SCREEN_W() );
    int y = utils::random( Conf::i().SCREEN_H() );
    input_->cursor().x() = x;
    input_->cursor().y() = y;
    input_->getCursor()->set<accessor::Pos2D>(vec2(x, y));

    return self();
}

void AIPlayer::think()
{
    if( !brain_->isThinking() && !is_executing_ && brain_->needThinking() && heat() < 0.75 ) {

        //Logger::i().buf("player ").buf(this).buf(" goes into thinking function.").endl();
        if( think_thread_ ) {
            //Logger::i().buf("player ").buf(this).buf(" call joining thread. ").endl();
            //think_thread_->join();
            //Logger::i().buf("player ").buf(this).buf(" thread joined. ").endl();
            //Logger::i().buf("player ").buf(this).buf(" think thread use count: ").buf(think_thread_.use_count()).endl();
        }

        std::vector< model::pSimpleMap > model_list;
        model_list.push_back( map_list_[0].lock()->model()->dump_data() );
        model_list.push_back( map_list_[1].lock()->model()->dump_data() );
        //Logger::i().buf("player ").buf(this).buf(" before creating thread, map ").buf(model_list[0]).buf(" is created").endl();

        think_thread_ = pThread(
            new boost::thread( bind(&model::AIBrain::think, brain_,
                                    model_list,
                                    ally_input_ids_,
                                    enemy_input_ids_) ));
    }
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

void AIPlayer::stopAllActions()
{
    Player::stopAllActions();
    if( think_timer_ && think_thread_ ) {
        std::cout << "AIPlayer stopped thinking." << std::endl;
        think_timer_.reset();
        think_thread_->join();
    }
}

void AIPlayer::issue_command( model::pAICommand const& cmd )
{
    typedef model::AICommand::pPosition pPosition;
    typedef model::AICommand::pButton   pButton;
    typedef model::AICommand::BtnID     BtnID;
    if( pButton btn = cmd->btn() ) {
        switch( *btn ) {
            case BtnID::TRIG_1:
                if( pPosition pos = cmd->pos() ) {
                    shoot( pos->first, pos->second );
                }
                break;
            case BtnID::TRIG_2:
                haste( 500 );
                break;
            default:
                break;
        }
    }
}

//you can only call this after setMapList is called
data::pViewSetting AIPlayer::view_setting() const
{
    if( presenter::pMap m = map_list_[id_].lock() )
        return m->view_setting();
    else return data::pViewSetting();
}

void AIPlayer::shoot(int x, int y) //we must know ViewSetting here.
{
    using namespace accessor;
    using namespace easing;
    int c_size = view_setting()->cube_size();
    vec2 dest(x*c_size + c_size/2 + view_setting()->x_offset(),
              -y*c_size - c_size/2 + view_setting()->y_offset());

    input_->cursor().x() = dest.X;
    input_->cursor().y() = dest.Y;

    function<void()> cb = bind(&AIPlayer::hold_button, this, ref(trig1_), 1);

    input_->getCursor()->tween<IOExpo, Pos2D>(dest, 200, 0, cb);
}

void AIPlayer::haste(int ms)
{
    hold_button(trig2_, ms);
}

void AIPlayer::hold_button(bool& corresponding_btn_state_, int ms)
{
    press_button(corresponding_btn_state_);

    pAIPlayer self = static_pointer_cast<AIPlayer>(shared_from_this());
    function<void()> cb = bind(&AIPlayer::release_button, this, ref(corresponding_btn_state_));

    EventDispatcher::i().subscribe_timer(cb, self, ms);
}

void AIPlayer::press_button(bool& corresponding_btn_state_)
{
    corresponding_btn_state_ = true;
}

void AIPlayer::release_button(bool& corresponding_btn_state_)
{
    corresponding_btn_state_ = false;
    is_executing_ = false; //this indicate executing finished.
}

void AIPlayer::cycle()
{
    input_->trig1().now() = trig1_;
    input_->trig2().now() = trig2_;

    if( !is_executing_ ) {
        if( model::pAICommand cmd = brain_->getCurrentCmd() ) {
            //Logger::i().buf("player ").buf(this).buf(" issuing command: ").buf(cmd).endl();
            is_executing_ = true; //this indicate executing started.
            issue_command( cmd );
            brain_->popCmdQueue();
            //Logger::i().buf("player ").buf(this).buf(" done issuing command: ").buf(cmd).endl();
        }
    }
}
