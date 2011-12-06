
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

AIPlayer::AIPlayer(Input* input, int const& id, AISetting const& setting)
    :Player(input, id), brain_(0), is_executing_(false), trig1_(false), trig2_(false),
     setting_(setting), think_interval_(setting.think_interval_ >= 100 ? setting.think_interval_ : 100)
{
}

AIPlayer::~AIPlayer()
{
    input_->getCursor()->clearAllTween(); //in case AIPlayer's hold_button is still called after player's death.
    input_->getCursor()->tween<easing::Linear, accessor::Rotation>
        (vec3(0,0,0), vec3(0,0,360), 3000u, -1); //replace it for original animation
    delete brain_;
}

pAIPlayer AIPlayer::init()
{
    Player::init();
    self_ = std::tr1::static_pointer_cast<AIPlayer>(shared_from_this());
    brain_ = new model::AIBrain(self());
    brain_->power(setting_.attack_power_);

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

        std::vector< model::pSimpleMap > ally_maps;
        std::vector< model::pSimpleMap > enemy_maps;
        for( std::list<int>::const_iterator i = ally_input_ids_.begin(); i != ally_input_ids_.end(); ++i ) {
            if( presenter::pMap m = map_list_[ *i ].lock() )
                ally_maps.push_back( m->model()->dump_data() );
        }
        for( std::list<int>::const_iterator i = enemy_input_ids_.begin(); i != enemy_input_ids_.end(); ++i ) {
            if( presenter::pMap m = map_list_[ *i ].lock() )
                enemy_maps.push_back( m->model()->dump_data() );
        }

        think_thread_ = pThread(
            new boost::thread( bind(&model::AIBrain::think, brain_,
                                    ally_maps,
                                    enemy_maps) ));
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

    int handshaking_x = utils::random(c_size/2) - c_size/4;
    int handshaking_y = utils::random(c_size/2) - c_size/4;

    if( utils::random(100) < setting_.missrate_ ) { //this shot is probably going to miss the target
        handshaking_x *= 4;
        handshaking_y *= 4;
    }

    vec2 dest(x*c_size + c_size/2 + view_setting()->x_offset() + handshaking_x,
              -y*c_size - c_size/2 + view_setting()->y_offset() + handshaking_y);
    input_->cursor().x() = dest.X;
    input_->cursor().y() = dest.Y;

    function<void()> cb = bind(&AIPlayer::hold_button, this, ref(trig1_), 1);

    input_->getCursor()->tween<IOExpo, Pos2D>(dest, think_interval_ - 100, 0, cb);
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
