
#include "presenter/Transitioner.hpp"
#include "view/Scene.hpp"
#include "view/Menu.hpp"

#include "Accessors.hpp"
#include "EasingEquations.hpp"
#include "IrrDevice.hpp"

using namespace irr;
using namespace core;
using namespace scene;
using namespace accessor;
using namespace easing;

using namespace psc;
using namespace presenter;

void Transitioner::init()
{
    transition_scene_ = view::Scene::create();
    transition_scene_->setTo2DView(640,480);

    loading_menu_ = view::Menu::create("loading", transition_scene_);
    loading_menu_->setDepth(3.0f).set<Size2D>(dimension2df(640,480));
    loading_menu_->addSprite("loading_bar").addSpriteText("Loading...", 0, "/fonts/arial.ttf", 72);
    loading_menu_->getSprite("loading_bar").setDepth(-1).set<RGBDiffuse>(128).
                   set<Pos2D>(vector2df(0, -200)).set<Size2D>(dimension2df(600, 20));
    loading_menu_->getSprite("Loading...").setDepth(-2);
}

Transitioner&
Transitioner::setLoadingBar(int const& percent)
{
    //ugly code here, damn it Billboards can't be scaled.
    float fper = static_cast<float>(percent) * 6;
    loading_menu_->getSprite("loading_bar").set<Size2D>(dimension2df(fper, 20));

    if( percent > 99 ) {
        loading_menu_->tween<Linear, Alpha>(0, 1000, false);
        loading_menu_->getSprite("loading_bar").tween<Linear, Alpha>(0, 1000, false);
        loading_menu_->getSprite("Loading...").set<Visible>(false);
    }

    return *this;
}

void Transitioner::cycle()
{
    //temp code, currently I can't think how to make it better.
    transition_scene_->activate();
    IrrDevice::i().d()->getSceneManager()->drawAll();
    transition_scene_->deactivate();
}
