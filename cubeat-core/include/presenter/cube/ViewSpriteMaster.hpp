#ifndef _SHOOTING_CUBES_PRESENTER_CUBE_VIEW_SPRITE_MASTER_
#define _SHOOTING_CUBES_PRESENTER_CUBE_VIEW_SPRITE_MASTER_

#include "presenter/cube/ViewMaster.hpp"
#include "utils/ObjectPool.hpp"

#include <deque>
#include <boost/tr1/memory.hpp>

namespace psc {

namespace data{
class ViewSetting;
typedef std::tr1::shared_ptr<ViewSetting> pViewSetting;
typedef std::tr1::weak_ptr<ViewSetting> wpViewSetting;
} //data

namespace ctrl{
class Player;
typedef std::tr1::shared_ptr<Player> pPlayer;
typedef std::tr1::weak_ptr<Player> wpPlayer;
} //ctrl

namespace model{
class Cube;
typedef std::tr1::shared_ptr<Cube> pCube;
typedef std::tr1::weak_ptr<Cube> wpCube;
class Chain;
typedef std::tr1::shared_ptr<Chain> pChain;
typedef std::tr1::weak_ptr<Chain> wpChain;
} //model

namespace view {
class Sprite;
class Menu;
class Scene;
class AnimatedSprite;
class SpriteText;
class SFX;
typedef std::tr1::shared_ptr<Sprite> pSprite;
typedef std::tr1::weak_ptr<Sprite> wpSprite;
typedef std::tr1::shared_ptr<Menu> pMenu;
typedef std::tr1::weak_ptr<Menu> wpMenu;
typedef std::tr1::shared_ptr<Scene> pScene;
typedef std::tr1::weak_ptr<Scene> wpScene;
typedef std::tr1::shared_ptr<AnimatedSprite> pAnimatedSprite;
typedef std::tr1::weak_ptr<AnimatedSprite> wpAnimatedSprite;
typedef std::tr1::shared_ptr<SpriteText> pSpriteText;
typedef std::tr1::weak_ptr<SpriteText> wpSpriteText;
} //view

namespace presenter {

class Map;

namespace cube {

class ViewSpriteMaster: public ViewMaster, public std::tr1::enable_shared_from_this<ViewSpriteMaster> {
public:
    typedef std::tr1::shared_ptr<ViewSpriteMaster> pointer_type;

    static ViewMaster::pointer_type create(view::pScene scene, data::pViewSetting setting, ctrl::wpPlayer const& player = ctrl::wpPlayer())
    {
        pointer_type result(new ViewSpriteMaster(scene, setting, player));
        return result;
    }
public:
    // no covariant return type
    virtual ViewBase::pointer_type create(model::pCube cube) const;
    virtual void column_full(int at);
    virtual void column_not_full(int at);
    virtual void new_chain(model::wpChain const& chain);
    virtual void new_garbage(std::vector< std::pair<int, int> > const& dying_cubes_position, int power);
    virtual void pop_garbage(int this_frame_lands);
    virtual void warning_sound(int warning_level);
    virtual void alert_bar_animate(int warning_level);
    virtual void alert_bar_freeze(bool freezed);
    virtual void alert_bar_update(int warning_level);
    virtual void show_overheat(bool show);
    virtual void ability_button(int left);

    virtual void setup_ability_button();

    virtual void cycle(Map const& map);

    void cleanup_chaintext();

private:
    ViewSpriteMaster(view::pScene scene, data::pViewSetting setting,
        ctrl::wpPlayer const& player);

private:
    virtual void derived_init();
    void update_garbage(int);
    void create_warning_strips();
    void create_overheat_overlay();
    void show_warning_at(int x, bool visible);
    void pop_a_chain_text(model::wpChain const& key);
    void invoke_ability(view::pSprite const& sp);

    virtual void new_garbage_2ndphase(vec2 const& pos, int new_count);

    vec2 garbage_endpoint_vec2() const;
    vec2 pos_vec2(int const& x, int const& y) const;
    static vec2 pos_vec2(data::pViewSetting const& setting, int const& x, int const& y);

private:
    view::wpScene scene_;
    ctrl::wpPlayer player_;
    view::pSprite overheat_;
    view::pSprite overheat_bg_;
    view::pSprite box_top_, box_bottom_, box_left_, box_right_, box_bg_;
    view::pSprite ability_btn_, alert_bar_cover_top_, alert_bar_cover_bottom_;
    view::pAnimatedSprite alert_bar_top_;
    view::pAnimatedSprite alert_bar_bottom_;
    view::pSpriteText garbage_text_, garbage_text_outline_;
    std::vector< view::pSprite > warning_strip_;
    std::vector< view::pSprite > warning_strip2_;
    std::map< model::wpChain, view::pMenu > chain_texts_;
    std::list< model::wpChain > chain_texts_to_be_deleted_;
    std::deque< view::pSprite > attack_cubes_;
    typedef std::pair<model::wpChain const, view::pMenu > ChainTextPair;
    typedef std::map< model::wpChain, view::pMenu >       ChainTextMap;

    int column_flag_;
    int i_have_to_keep_track_of_garbage_count_visually_here_;
};

} //cube
} //presenter
} //psc

#endif //_SHOOTING_CUBES_PRESENTER_CUBE_VIEW_SPRITE_MASTER_
