
#include "presenter/Transitioner.hpp"
#include "view/Scene.hpp"
#include "view/Menu.hpp"
#include "data/Color.hpp"
#include "Conf.hpp"

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
                  .addSpriteText("text1", "... loading ...", "Star Jedi", 0, 60, true)
                  .addSpriteText("text2", "... please wait ...", "Star Jedi", 0, 36, true);
    loading_menu_->getSprite("loading_bar").moveTo(20, 440)
                  .set<GradientDiffuse>(128);
    loading_menu_->getSprite("text1").moveTo(320, 204);
    loading_menu_->getSprite("text2").moveTo(320, 276);

    loading_menu_->set<Visible>(false);
}

Transitioner&
Transitioner::setLoadingBar(int const& percent)
{
    loading_menu_->set<Visible>(true);
    loading_menu_->getSprite("loading_bar").set<Scale>(vec3(percent/100.0f,1,1));

    if( percent > 99 ) {
        function<void()> endcall = bind(&view::Object::set<Visible>, loading_menu_.get(), false);
        loading_menu_->tween<Linear, Alpha>(0, 2000, false, endcall);
        loading_menu_->getSprite("loading_bar").tween<Linear, Alpha>(0, 1000, false);
        loading_menu_->getSprite("text1").tween<Linear, Alpha>(0, 1000, false);
        loading_menu_->getSprite("text2").tween<Linear, Alpha>(0, 1000, false);
    }
    return *this;
}

void Transitioner::cycle()
{
    transition_scene_->activate().redraw();
    transition_scene_->deactivate();
}
