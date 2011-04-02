

#ifndef _SHOOTING_CUBES_CTRL_PLAYER_
#define _SHOOTING_CUBES_CTRL_PLAYER_

#include "all_fwd.hpp"
#include "utils/dictionary.hpp"
#include <vector>
#include <list>
#include <boost/tr1/functional.hpp>

namespace psc {
namespace ctrl {

class Input;
class Weapon;

class Player : public std::tr1::enable_shared_from_this<Player>
{
    typedef std::tr1::function<void(view::pSprite&)> ObjCallback;
    typedef std::tr1::function<void(int)> HitCallback;

public:
    typedef std::tr1::shared_ptr< Player > pointer_type;
    typedef std::tr1::weak_ptr  < Player > wpointer_type;
    static pointer_type create(Input* input,
                               data::pViewSetting const& view_setting) {
        return pointer_type(new Player(input, view_setting))->init();
    }
    virtual void cycle();
    virtual bool startThinking() { return false; }
    virtual void stopThinking() {}
    void setMapList(std::vector<presenter::wpMap> const& mlist) { map_list_ = mlist; }

    Player& set_active_weapon(int i);
    Player& debug_reset_all_weapon();
    Player& set_config(utils::map_any const& config);
    Player& disable_all_wep_reloadability();

    Player& subscribe_shot_event(view::pSprite&, HitCallback const&, HitCallback const& enemy_cb = 0);
    //I'd better refactor this afterwards.
    void eat_item();

    Input const* input()          const;
    Weapon* weapon()              const;
    Weapon* weapon(int id)        const;
    bool is_changing_wep()        const;
    bool can_fire()               const;
    bool can_crossfire()          const;
    bool can_fire_repeatedly()    const;
    int  wepid()                  const;
    bool ammo_all_out()           const;
    double heat()                 const;
    bool is_overheat()            const;
    int  overheat_downtime()      const;

    void generate_heat(double);
    float haste_speedfunc(float orig_speed) const;

    virtual ~Player();

protected:
    Player(Input* input, data::pViewSetting const&);
    pointer_type init();

    void normal_weapon_fx();
    void start_haste_effect();
    void remove_haste_effect();
    void normal_shot_delegate(view::pSprite&, HitCallback const&);
    void shot_delegate(view::pSprite&, HitCallback const&, wpointer_type);
    void repeating_shot_delegate(view::pSprite&, HitCallback const&, wpointer_type);
    void process_input();
    void heat_cooling();

protected:
    int const changetime_;
    bool      changing_wep_;
    int       weplist_idx_;
    double    accumulated_heat_, cooling_speed_, heat_for_normal_shoot_, heat_for_haste_, heat_for_jama_shoot_;
    int       overheat_downtime_;
    bool      overheat_, hasting_;

    Input*               input_;
	Weapon*              current_wep_;
	data::pViewSetting   view_setting_;
	std::vector<Weapon*> weplist_;
	std::vector<presenter::wpMap> map_list_;
};

typedef std::tr1::shared_ptr<Player> pPlayer;
typedef std::tr1::weak_ptr<Player> wpPlayer;

} //ctrl
} //psc

#endif //_SHOOTING_CUBES_CTRL_PLAYER_
