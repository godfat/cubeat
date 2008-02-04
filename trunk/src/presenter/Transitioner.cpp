
#include "presenter/Transitioner.hpp"
#include "view/Scene.hpp"
#include "view/Menu.hpp"
#include "data/Color.hpp"

#include "Accessors.hpp"
#include "EasingEquations.hpp"
#include "IrrDevice.hpp"

#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace accessor;
using namespace easing;

using namespace psc;
using namespace presenter;

using std::tr1::bind;
using std::tr1::ref;
using std::tr1::function;

void Transitioner::init()
{
    transition_scene_ = view::Scene::create();
    transition_scene_->setTo2DView(640,480);

    loading_menu_ = view::Menu::create("loading", transition_scene_);
    loading_menu_->setDepth(3.0f).set<Size2D>(dimension2df(640,480)).set<GradientDiffuse>(0);
    loading_menu_->addSprite("loading_bar")
                  .addSpriteText("Loading...", 0, "fonts/arial.ttf", 72, data::Color(255,255,255));
    loading_menu_->getSprite("loading_bar").setDepth(-1).moveTo(20, 440).
                   set<GradientDiffuse>(128).set<Size2D>(dimension2df(600, 20));
    loading_menu_->getSprite("Loading...").setDepth(-2).moveTo(320, 240).setCenterAligned(true);
}

Transitioner&
Transitioner::setLoadingBar(int const& percent)
{
    float fper = static_cast<float>(percent) * 6;
    loading_menu_->getSprite("loading_bar").set<Size2D>(dimension2df(fper, 20));

    if( percent > 99 ) {
        function<void()> endcall = bind(&view::Object::set<Visible>, loading_menu_.get(), false);
        loading_menu_->tween<Linear, Alpha>(0, 1000, false, endcall);
        loading_menu_->getSprite("loading_bar").tween<Linear, Alpha>(0, 1000, false);
        loading_menu_->getSprite("Loading...").tween<Linear, Alpha>(0, 1000, false);
    }

    return *this;
}

void Transitioner::cycle()
{
    transition_scene_->activate().redraw();
    transition_scene_->deactivate();
}
