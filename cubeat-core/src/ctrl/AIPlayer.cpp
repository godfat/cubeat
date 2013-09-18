
#include "ctrl/AIPlayer.hpp"
#include "data/ViewSetting.hpp"
#include "model/AICommand.hpp"
#include "model/SimpleMap.hpp"
#include "presenter/Map.hpp"
#include "view/Sprite.hpp"
#include "EventDispatcher.hpp"
#include "ctrl/TimerDispatcher.hpp"
#include "Input.hpp"
#include "Accessors.hpp"
#include "EasingEquations.hpp"
#include "utils/Logger.hpp"
#include "utils/Random.hpp"
#include "script/lua_utility.hpp"
#include "Conf.hpp"

#include <boost/tr1/functional.hpp>

using namespace psc;
using namespace ctrl;
using std::tr1::bind;
using std::tr1::static_pointer_cast;
using std::tr1::ref;
using std::tr1::function;
using std::tr1::tuple;
using utils::Logger;

AIPlayer::AIPlayer(Input* input, int const& id, std::string const& ai_name)
    :Player(input, id), is_executing_(false), trig1_(false), trig2_(false),
     ai_name_(ai_name), think_interval_(350), missrate_(10)
{
}

AIPlayer::~AIPlayer()
{
    input_->getCursor()->clearAllTween(); //in case AIPlayer's hold_button is still called after player's death.
    input_->getCursor()->tween<easing::Linear, accessor::Rotation>
        (vec3(0,0,0), vec3(0,0,360), 3000u, -1); //replace it for original animation

    lua_close(L_);
}

pAIPlayer AIPlayer::init()
{
    Player::init();
    self_ = std::tr1::static_pointer_cast<AIPlayer>(shared_from_this());

    int x = utils::random( Conf::i().SCREEN_W() );
    int y = utils::random( Conf::i().SCREEN_H() );
    input_->cursor().x() = x;
    input_->cursor().y() = y;
    input_->getCursor()->set<accessor::Pos2D>(vec2(x, y));

    std::cout << "AI processing unit created." << std::endl;
    L_ = luaL_newstate();
    luaL_openlibs(L_);
    script::Lua::run_script(L_, Conf::i().script_path(ai_name_).c_str());

    think_interval_ = script::Lua::call_R<int>(L_, "THINK_INTERVAL");
    missrate_       = script::Lua::call_R<int>(L_, "MISSRATE");

    return self();
}

void AIPlayer::setMapList(std::vector<presenter::wpMap> const& mlist)
{
    map_list_ = mlist;
    update_map_models();
}

void AIPlayer::update_map_models()
{
    ally_maps_.clear();
    enemy_maps_.clear();
    for( std::list<int>::const_iterator i = ally_input_ids_.begin(); i != ally_input_ids_.end(); ++i ) {
        if( presenter::pMap m = map_list_[ *i ].lock() )
            ally_maps_.push_back( m->model()->dump_data() );
    }
    for( std::list<int>::const_iterator i = enemy_input_ids_.begin(); i != enemy_input_ids_.end(); ++i ) {
        if( presenter::pMap m = map_list_[ *i ].lock() )
            enemy_maps_.push_back( m->model()->dump_data() );
    }
}

void AIPlayer::open_thread_to_think()
{
    update_map_models();
    //note: heat and cmd_queue_ size should be considered INSIDE thinking process.
    typedef boost::posix_time::milliseconds ms;
    if( (!think_thread_ || (think_thread_ && think_thread_->timed_join(ms(0))) ) && //memo: FUCK YOU.
        !is_executing_ )
    {
        think_thread_ = pThread( new boost::thread( bind(&AIPlayer::think, this) ) );
    }
}

void AIPlayer::think()
{
    script::Lua::call(L_, "ai_entry", static_cast<void*>(this));
}

bool AIPlayer::startThinking()
{
    if( !think_thread_ ) {
        std::cout << "CPU AI started thinking ..." << std::endl;
        think_timer_ = pDummy(new int);

        //2012 BAD WTF, but for ability test here:
        if( ability_kind_ == 1 ) think_interval_ *= 0.925;
        std::cout << " -- think_interval_: " << think_interval_ << "\n";

        EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
            bind( &AIPlayer::open_thread_to_think, this), think_timer_, think_interval_, -1);
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
        think_thread_.reset();
    }
}

void AIPlayer::issue_command( model::pAICommand const& cmd )
{
    using model::AICommand;
    typedef AICommand::pPosition pPosition;
    pAIPlayer self = static_pointer_cast<AIPlayer>(shared_from_this());

    switch( cmd->type() ) {
        case AICommand::SHOOT:
        case AICommand::SHOOT_OTHER:
        case AICommand::USE_ABILITY:
            if( pPosition pos = cmd->pos() ) {
                if( cmd->delay() < 1 ) {
                    shoot( pos->first, pos->second, cmd->type() );
                } else {
                    EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
                        bind(&AIPlayer::shoot, this, pos->first, pos->second, cmd->type()), self, cmd->delay());
                }
            }
            break;
        case AICommand::HASTE:
            if( cmd->delay() < 1 ) {
                haste( 500 );
            } else {
                printf("Issue command delay: %d\n", cmd->delay());
                EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
                    bind(&AIPlayer::haste, this, 500), self, cmd->delay());
            }
            break;
        default:
            break;
    }
}

//you can only call this after setMapList is called
data::pViewSetting AIPlayer::view_setting(bool const& self) const
{
    int id = self ? id_ : enemy_input_ids_.front();
    if( presenter::pMap m = map_list_[id].lock() )
        return m->view_setting();
    else return data::pViewSetting();
}

void AIPlayer::shoot(int x, int y, int type) //we must know ViewSetting here.
{
    using namespace accessor;
    using namespace easing;

    vec2 dest;
    switch( type ) {
        case model::AICommand::SHOOT: {
            int c_size = view_setting(true)->cube_size();
            dest.X =  x*c_size + c_size/2 + view_setting(true)->x_offset();
            dest.Y = -y*c_size - c_size/2 + view_setting(true)->y_offset();
            break;
        }
        case model::AICommand::SHOOT_OTHER: {
            int c_size = view_setting(false)->cube_size();
            dest.X =  x*c_size + c_size/2 + view_setting(false)->x_offset();
            dest.Y = -y*c_size - c_size/2 + view_setting(false)->y_offset();
            break;
        }
        case model::AICommand::USE_ABILITY:
            dest.X = view_setting(true)->abl_btn_x();
            dest.Y = view_setting(true)->abl_btn_y();
            std::cout << "AIPlayer::shooting ability button at " << dest.X << ", " << dest.Y << "\n";
            break;
        default:
            std::cerr << "AIPlayer::shoot wrong code flow.\n";
            break;
    }

    vec2 curr(input_->cursor().x(), input_->cursor().y());
    double dist = (dest - curr).getLength() + 32; //add a very least handshaking possibility

    int handshaking_x = utils::random(static_cast<int>(dist*0.07)) - static_cast<int>(dist*0.035);
    int handshaking_y = utils::random(static_cast<int>(dist*0.07)) - static_cast<int>(dist*0.035);
    if( utils::random(100) < missrate_ ) { //this shot is probably going to miss the target
        handshaking_x *= 5;
        handshaking_y *= 5;
    }

    dest.X += handshaking_x;
    dest.Y += handshaking_y;

    input_->cursor().x() = dest.X;
    input_->cursor().y() = dest.Y;

    function<void()> cb = bind(&AIPlayer::hold_button, this, ref(trig1_), 1);

    int mov_duration = think_interval_ - 100;
    if ( type == model::AICommand::SHOOT_OTHER )
        mov_duration *= 1.5; //interrupt your opponents should be more costly

    input_->getCursor()->tween<IOExpo, Pos2D>(dest, mov_duration, 0, cb);
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

    EventDispatcher::i().get_timer_dispatcher("game")->subscribe(cb, self, ms);
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

model::pAICommand AIPlayer::getCurrentCmd()
{
    if( !cmd_queue_.empty() ) {
        return cmd_queue_.front();
    }
    else
        return model::pAICommand();
}

void AIPlayer::popCmdQueue()
{
    if( !cmd_queue_.empty() ) {
        cmd_queue_.pop_front();
    }
}

//scripting usage only
int AIPlayer::cmdQueueSize() {
    return cmd_queue_.size();
}

void AIPlayer::pushCommand(model::pAICommand const& cmd) {
    cmd_queue_.push_back( cmd );
}

model::pSimpleMap AIPlayer::getAllyMap (size_t const& index) {
    return ( index < ally_maps_.size() ) ? ally_maps_[index] : model::pSimpleMap();
}

model::pSimpleMap AIPlayer::getEnemyMap(size_t const& index) {
    return ( index < enemy_maps_.size() ) ? enemy_maps_[index] : model::pSimpleMap();
}

void AIPlayer::cycle()
{
    input_->trig1().now() = trig1_;
    input_->trig2().now() = trig2_;

    if( !is_executing_ ) {
        if( model::pAICommand cmd = getCurrentCmd() ) {
            //Logger::i().buf("player ").buf(this).buf(" issuing command: ").buf(cmd).endl();
            is_executing_ = true; //this indicate executing started.
            issue_command( cmd );
            popCmdQueue();
            //Logger::i().buf("player ").buf(this).buf(" done issuing command: ").buf(cmd).endl();
        }
    }
}
