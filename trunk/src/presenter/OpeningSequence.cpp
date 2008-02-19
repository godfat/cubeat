
#include "presenter/OpeningSequence.hpp"
#include "view/SpriteMovie.hpp"
#include "view/Scene.hpp"

#include "Accessors.hpp"
#include "EasingEquations.hpp"
#include "App.hpp"

#include <iostream>

using namespace psc;
using namespace presenter;
using namespace accessor;
using namespace easing;

using std::tr1::bind;
using std::tr1::ref;
using std::tr1::function;

OpeningSequence::OpeningSequence()
    :deletion_scheduled_(false)
{
}

OpeningSequence::~OpeningSequence()
{
    std::cout << "Did Opening died properly?\n";
}

pOpeningSequence OpeningSequence::init()
{
    scene_ = view::Scene::create(view::pObject(), "OpeningSequence");
    scene_->setTo2DView();
    movie_ = view::SpriteMovie::create("E:/portfolio/ndssp/ndssp_op.avi", scene_, 640, 480);
    movie_->setCurrentFrame(900); //temp
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
    scene_->activate().redraw();
    scene_->deactivate();

    if( !deletion_scheduled_ && movie_->isEnd() ) {
        std::cout << "OpeningSequence deletion scheduled.\n";
        function<void()> endcall = bind(&OpeningSequence::end, this);
        movie_->tween<Linear, GradientDiffuse>(0, 2000, false, endcall);
        deletion_scheduled_ = true;
    }
}
