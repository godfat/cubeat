
#include "presenter/OpeningSequence.hpp"
#include "view/SpriteMovie.hpp"
#include "view/Scene.hpp"

#include "Accessors.hpp"
#include "EasingEquations.hpp"
#include "App.hpp"
#include "Conf.hpp"
#include "Input.hpp"

#include <iostream>

using namespace psc;
using namespace presenter;
using namespace accessor;
using namespace easing;

using std::tr1::bind;
using std::tr1::ref;
using std::tr1::function;
using namespace std::tr1::placeholders;

OpeningSequence::OpeningSequence()
    :deletion_scheduled_(false)
{
    config = utils::map_any::construct(
        utils::fetchConfig( Conf::i().CONFIG_PRESENTER_PATH +"opening.zzml" ) );
}

OpeningSequence::~OpeningSequence()
{
    std::cout << "Did Opening died properly?\n";
}

pOpeningSequence OpeningSequence::init()
{
    scene_ = view::Scene::create("OpeningSequence");
    scene_->setTo2DView();
    movie_ = view::SpriteMovie::create(config.S("movie_path"), scene_,
                                       Conf::i().SCREEN_W, Conf::i().SCREEN_H);
    movie_->setCurrentFrame( config.I("startframe") ); //temp

    return shared_from_this();
}

void OpeningSequence::end()
{
    movie_->set<Visible>(false);
    App::i().launchMainMenu();
    std::cout << "OpeningSequence end call finished.\n";
}

void OpeningSequence::cycle()
{
    movie_->redraw();
    scene_->redraw();

    if( !deletion_scheduled_ && movie_->isEnd() ) {
        std::cout << "OpeningSequence deletion scheduled.\n";
        function<void()> endcall = bind(&OpeningSequence::end, this);
        movie_->tween<Linear, GradientDiffuse>(0, config.I("fade_period"), false, endcall);
        deletion_scheduled_ = true;
    }
}
