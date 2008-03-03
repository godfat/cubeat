
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
    config = utils::map_any::construct(
        utils::fetchConfig( Conf::i().CONFIG_PRESENTER_PATH +"transitioner.zzml" ) );

    utils::map_any& text1= config.M("text1");
    utils::map_any& text2= config.M("text2");
    int const b_text_size = Conf::i().SCREEN_W/20;
    int const m_text_size = Conf::i().SCREEN_W/30;

    transition_scene_ = view::Scene::create("Transitioner");
    transition_scene_->setTo2DView();

    loading_menu_ = view::Menu::create("loading", transition_scene_,
                                       Conf::i().SCREEN_W, Conf::i().SCREEN_H);

    loading_menu_->set<GradientDiffuse>(0);
    loading_menu_->addSprite("loading_bar", 0, Conf::i().SCREEN_W - 40, Conf::i().SCREEN_H / 20 )
                  .addSpriteText("text1", text1.S("text"), text1.S("font"),
                                 0, b_text_size, true)
                  .addSpriteText("text2", text2.S("text"), text2.S("font"),
                                 0, m_text_size, true);
    loading_menu_->getSprite("loading_bar").moveTo( 20, Conf::i().SCREEN_H / 10 * 9 )
                  .set<GradientDiffuse>(128);
    loading_menu_->getSprite("text1")
                  .moveTo( Conf::i().SCREEN_W/2, Conf::i().SCREEN_H/2 - b_text_size );
    loading_menu_->getSprite("text2")
                  .moveTo( Conf::i().SCREEN_W/2, Conf::i().SCREEN_H/2 + m_text_size );

    loading_menu_->set<Visible>(false);
}

Transitioner&
Transitioner::setLoadingBar(int const& percent)
{
    loading_menu_->set<Visible>(true);
    loading_menu_->getSprite("loading_bar").set<Scale>(vec3(percent/100.0f,1,1));

    if( percent > 99 ) {
        function<void()> endcall = bind(&view::Object::set<Visible>, loading_menu_.get(), false);
        loading_menu_->tween<Linear, Alpha>(0, config.M("background").I("fade_period"), false, endcall);
        loading_menu_->getSprite("loading_bar")
                      .tween<Linear, Alpha>(0, config.M("bar_setting").I("fade_period"), false);
        loading_menu_->getSprite("text1")
                      .tween<Linear, Alpha>(0, config.M("text1").I("fade_period"), false);
        loading_menu_->getSprite("text2")
                      .tween<Linear, Alpha>(0, config.M("text2").I("fade_period"), false);
    }
    return *this;
}

void Transitioner::cycle()
{
    transition_scene_->redraw();
}
