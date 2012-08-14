
#include "presenter/OpeningSequence.hpp"
#include "view/SpriteMovie.hpp"
#include "view/Scene.hpp"
#include "EventDispatcher.hpp"
#include "ctrl/TimerDispatcher.hpp"

#include "Accessors.hpp"
#include "EasingEquations.hpp"
#include "App.hpp"
#include "Conf.hpp"

#include <iostream>

using namespace psc;
using namespace presenter;
using namespace accessor;
using namespace easing;

using std::tr1::bind;
using std::tr1::ref;
using std::tr1::function;
using namespace std::tr1::placeholders;

/********************************************/
/// Movie related codes have been commented.*/
/********************************************/

OpeningSequence::OpeningSequence()
    :deletion_scheduled_(false)
{
    config_ = Conf::i().config_of("presenter/opening");
}

OpeningSequence::~OpeningSequence()
{
    irr::video::IVideoDriver* driver = IrrDevice::i().d()->getVideoDriver();
    //the startup page texture isn't going to be useful anymore.
    driver->removeTexture( startup_page_->body()->getMaterial(0).getTexture(0) );
    std::cout << "Did Opening died properly?" << std::endl;
}

pOpeningSequence OpeningSequence::init()
{
    scene_ = view::Scene::create("opening");
    scene_->setTo2DView();
    int width  = Conf::i().SCREEN_W();
    int height = Conf::i().SCREEN_H();
    int duration    = config_.I("startup_time");
    int fade_period = config_.I("fade_period");
    startup_page_ = view::Sprite::create(config_.S("startup_image_path"), scene_, width, height, true);
    startup_page_->set<GradientDiffuse>(0).set<Pos2D>(vec2(width/2, height/2));
    using data::AnimatorParam;
    AnimatorParam<Linear, GradientDiffuse> anim1;
    AnimatorParam<Linear, GradientDiffuse> anim2;
    anim1.start(0).end(255).delay(fade_period*2).duration(fade_period);
    anim2.start(255).end(0).duration(fade_period).delay(duration - fade_period*4);
    startup_page_->queue(anim1).tween(anim2);
//    movie_ = view::SpriteMovie::create(config_.S("movie_path"), scene_,
//                                       Conf::i().SCREEN_W(), Conf::i().SCREEN_H());
    ctrl::EventDispatcher::i().get_timer_dispatcher("opening")->subscribe(
        bind(&App::setLoading, &App::i(), 10), shared_from_this(), duration);
    ctrl::EventDispatcher::i().get_timer_dispatcher("opening")->subscribe(
        bind(&OpeningSequence::end, this), shared_from_this(), duration + 100); //+100 ms to make sure this happens later
    return shared_from_this();
}

void OpeningSequence::end()
{
//    movie_->set<Visible>(false);
    App::i().launchMainMenu();
    std::cout << "OpeningSequence end call finished." << std::endl;
}

void OpeningSequence::cycle()
{
//    movie_->redraw();
    scene_->redraw();

//    if( !deletion_scheduled_ && movie_->isEnd() ) {
//        std::cout << "OpeningSequence deletion scheduled." << std::endl;
//        function<void()> endcall = bind(&OpeningSequence::end, this);
//        movie_->tween<Linear, GradientDiffuse>(0, config_.I("fade_period"), 0, endcall);
//        deletion_scheduled_ = true;
//    }
}
