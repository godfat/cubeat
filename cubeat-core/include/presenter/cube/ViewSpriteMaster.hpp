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

class ViewSpriteMaster: public ViewMaster{
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
    virtual void new_garbage(model::wpChain const& chain, int n);
    virtual void pop_garbage(int amount);
    virtual void warning_counting(int warning_level);
    virtual void alert_bar_update(int warning_level);
    virtual void show_overheat(bool show);

    virtual void cycle(Map const& map);

    void cleanup_chaintext();

private:
    ViewSpriteMaster(view::pScene scene, data::pViewSetting setting,
        ctrl::wpPlayer const& player);

private:
    virtual void derived_init();
    void create_warning_strips();
    void create_overheat_overlay();
    void show_warning_at(int x, bool visible);
    void pop_a_chain_text(model::wpChain const& key);

    vec2 garbage_endpoint_vec2() const;
    vec2 pos_vec2(int const& x, int const& y) const;
    static vec2 pos_vec2(data::pViewSetting const& setting, int const& x, int const& y);

private:
    view::wpScene scene_;
    ctrl::wpPlayer player_;
    view::pSprite overheat_;
    view::pSprite overheat_bg_;
    view::pAnimatedSprite alert_bar_top_;
    view::pAnimatedSprite alert_bar_bottom_;
    std::vector< view::pSprite > warning_strip_;
    std::map< model::wpChain, view::pMenu > chain_texts_;
    std::list< model::wpChain > chain_texts_to_be_deleted_;
    std::deque< view::pSprite > attack_cubes_;
    typedef std::pair<model::wpChain const, view::pMenu > ChainTextPair;
    typedef std::map< model::wpChain, view::pMenu >       ChainTextMap;
};

} //cube
} //presenter
} //psc

#endif //_SHOOTING_CUBES_PRESENTER_CUBE_VIEW_SPRITE_MASTER_