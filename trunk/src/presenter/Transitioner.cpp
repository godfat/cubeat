
#include "presenter/Transitioner.hpp"
#include "view/Scene.hpp"
#include "view/Menu.hpp"
#include "data/Color.hpp"

#include "Accessors.hpp"
#include "EasingEquations.hpp"

using namespace psc;
using namespace presenter;
using namespace accessor;
using namespace easing;

using std::tr1::bind;
using std::tr1::ref;
using std::tr1::function;

void Transitioner::init()
{
    transition_scene_ = view::Scene::create(view::pObject(), "Transitioner");
    transition_scene_->setTo2DView();

    loading_menu_ = view::Menu::create("loading", transition_scene_, 640, 480);
    loading_menu_->set<GradientDiffuse>(0);
    loading_menu_->addSprite("loading_bar", 0, 600, 20)
                  .addSpriteText("what this font", "Star Jedi", 0, 48, true, data::Color(255,255,255))
                  .addSpriteText("looks like", "Star Jedi", 0, 36, true, data::Color(128,128,128));
    loading_menu_->getSprite("loading_bar").moveTo(20, 440)
                  .set<GradientDiffuse>(128);
    loading_menu_->getSprite("what this font").moveTo(320, 100)
                  .tween<Linear, Rotation>(vec3(0,0,360), 3000);
    loading_menu_->getSprite("looks like").moveTo(200,320).set<Rotation>(vec3(0,0,45));

    loading_menu_->set<Visible>(false);
}

Transitioner&
Transitioner::setLoadingBar(int const& percent)
{
    loading_menu_->set<Visible>(true);
    loading_menu_->getSprite("loading_bar").set<Scale>(vec3(percent/100.0f,1,1));

    if( percent == 1 )
        loading_menu_->getSprite("looks like").tween<OBounce, Size2D>(vec2(400, 100), 1000, false);

    if( percent > 99 ) {
        function<void()> endcall = bind(&view::Object::set<Visible>, loading_menu_.get(), false);
        loading_menu_->tween<Linear, Alpha>(0, 1000, false, endcall);
        loading_menu_->getSprite("loading_bar").tween<Linear, Alpha>(0, 1000, false);
        loading_menu_->getSprite("what this font").tween<Linear, Alpha>(0, 1000, false);
        loading_menu_->getSprite("looks like").tween<Linear, Alpha>(0, 1000, false);
    }
    return *this;
}

void Transitioner::cycle()
{
    transition_scene_->activate().redraw();
    transition_scene_->deactivate();
}
