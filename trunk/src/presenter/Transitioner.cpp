
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
    transition_scene_->setTo2DView();

    loading_menu_ = view::Menu::create("loading", transition_scene_, 640, 480);
    loading_menu_->setDepth(3).set<GradientDiffuse>(0);
    loading_menu_->addSprite("loading_bar", 0, 600, 20)
                  .addSpriteText("what this font", 0, "Star Jedi", 48, true, data::Color(255,255,255))
                  .addSpriteText("looks like", 0, "Star Jedi", 36, true, data::Color(128,128,128));
    loading_menu_->getSprite("loading_bar").setDepth(-1).moveTo(20, 440)
                  .set<GradientDiffuse>(128);
    loading_menu_->getSprite("what this font").setDepth(0).moveTo(320, 240)
                  .tween<Linear, Rotation>(vector3df(0,0,90), 3000);
    loading_menu_->getSprite("looks like").setDepth(0).moveTo(200,320).set<Rotation>(vector3df(0,0,45))
                  .tween<OBounce, Size2D>(vector2df(400, 100), 1234, false);
}

Transitioner&
Transitioner::setLoadingBar(int const& percent)
{
    loading_menu_->getSprite("loading_bar").set<Scale>(vector3df(percent/100.0f,1,1));

    if( percent > 99 ) {
        function<void()> endcall = bind(&view::Object::set<Visible>, loading_menu_.get(), false);
        loading_menu_->tween<Linear, Alpha>(0, 1000, false, endcall);
        loading_menu_->getSprite("loading_bar").tween<Linear, Alpha>(0, 1000, false);
        loading_menu_->getSprite("what this font").tween<Linear, Alpha>(0, 1000, false);
    }
    return *this;
}

void Transitioner::cycle()
{
    transition_scene_->activate().redraw();
    transition_scene_->deactivate();
}
