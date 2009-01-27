
#include "ctrl/AIPlayer.hpp"
#include "data/ViewSetting.hpp"
#include "model/AIBrain.hpp"
#include "presenter/Map.hpp"
#include "view/Sprite.hpp"
#include "EventDispatcher.hpp"
#include "Input.hpp"
#include "Accessors.hpp"
#include "EasingEquations.hpp"
#include "utils/Logger.hpp"

#include <tr1/functional>

using namespace psc;
using namespace ctrl;
using std::tr1::bind;
using std::tr1::static_pointer_cast;
using std::tr1::ref;
using std::tr1::function;
using utils::Logger;

AIPlayer::AIPlayer(Input* input, data::pViewSetting const& view_setting)
    :Player(input, view_setting), brain_(0), think_interval_(400), is_executing_(false)
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

    input_->cursor().x() = view_setting_->x_offset();
    input_->cursor().y() = view_setting_->y_offset(); //designate cursor initial point.
    input_->getCursor()->set<accessor::Pos2D>(vec2(view_setting_->x_offset(), view_setting_->y_offset()));

    return self();
}

void AIPlayer::think()
{
    if( !brain_->isThinking() && brain_->needThinking() ) {

        Logger::i().buf("player ").buf(this).buf(" goes into thinking function.").endl();
        if( think_thread_ ) {
            Logger::i().buf("player ").buf(this).buf(" call joining thread. ").endl();
            think_thread_->join();
            Logger::i().buf("player ").buf(this).buf(" thread joined. ").endl();
        }

        std::vector< model::pSimpleMap > model_list;
        model_list.push_back( map_list_[0].lock()->model()->dump_data() );
        model_list.push_back( map_list_[1].lock()->model()->dump_data() );
        Logger::i().buf("player ").buf(this).buf(" before creating thread, map ").buf(model_list[0]).buf(" is created").endl();

        think_thread_ = pThread(
            new boost::thread( bind(&model::AIBrain::think, brain_,
                                    model_list,
                                    view_setting_->ally_input_ids(),
                                    view_setting_->enemy_input_ids()) ));
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

void AIPlayer::stopThinking()
{
    if( think_timer_ && think_thread_ ) {
        std::cout << "AIPlayer stopped thinking." << std::endl;
        think_timer_.reset();
        think_thread_->join();
    }
}

void AIPlayer::shoot(int x, int y) //we must know ViewSetting here.
{
    using namespace accessor;
    using namespace easing;
    int c_size = view_setting_->cube_size();
    vec2 dest(x*c_size + c_size/2 + view_setting_->x_offset(),
              -y*c_size - c_size/2 + view_setting_->y_offset());

    input_->cursor().x() = dest.X;
    input_->cursor().y() = dest.Y;

    function<void()> cb = bind(&AIPlayer::hold_button, this, ref(input_->trig1()), 1);

    input_->getCursor()->tween<IOExpo, Pos2D>(dest, 200, 0, cb);
}

void AIPlayer::hold_button(ctrl::Button& btn_ref, int ms)
{
    press_button(btn_ref);

    pAIPlayer self = static_pointer_cast<AIPlayer>(shared_from_this());
    function<void()> cb = bind(&AIPlayer::release_button, this, ref(btn_ref));

    EventDispatcher::i().subscribe_timer(cb, self, ms);
}

void AIPlayer::press_button(ctrl::Button& btn_ref)
{
    btn_ref.now() = true;
}

void AIPlayer::release_button(ctrl::Button& btn_ref)
{
    btn_ref.now() = false;
    is_executing_ = false; //this indicate executing finished.
}

AIPlayer::pPosition AIPlayer::probing_brain_data()
{
    return brain_->getCurrentCmd();
}

void AIPlayer::cycle()
{
    if( !is_executing_ ) {
        if( pPosition pos = probing_brain_data() ) {
            is_executing_ = true; //this indicate executing started.
            shoot( pos->first, pos->second );
            brain_->popCmdQueue();
        }
        input_->haste().now() = true;
    }
    else
        input_->haste().now() = false;
}
