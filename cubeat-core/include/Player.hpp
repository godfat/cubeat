

#ifndef _SHOOTING_CUBES_CTRL_PLAYER_
#define _SHOOTING_CUBES_CTRL_PLAYER_

#include "all_fwd.hpp"
#include "utils/dictionary.hpp"
#include <vector>
#include <deque>
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
                               int const& id) {
        return pointer_type(new Player(input, id))->init();
    }
    virtual void cycle();
    virtual bool startThinking() { return false; }
    virtual void stopAllActions() { if( hasting_ ) remove_haste_effect(); }
    void setMapList(std::vector<presenter::wpMap> const& mlist);

    Player& set_active_weapon(int i);
    Player& debug_reset_all_weapon();
    Player& set_config(utils::map_any const& config);
    Player& disable_all_wep_reloadability();
    Player& push_ally(int id);
    Player& push_enemy(int id);
    Player& start_heat_timer();
    Player& subscribe_player_specific_interactions(bool const& can_haste = true);
    Player& subscribe_shot_event(view::pSprite&, HitCallback const&, HitCallback const& enemy_cb = 0);

    //I'd better refactor this afterwards.
    Player& invoke_ability(view::pSprite const&);
    Player& push_ability(int kind = -1); // -1 will just resolve to default
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
    bool is_hasting()             const;
    int  overheat_downtime()      const;
    int  id()                     const;
    int  ability_kind()           const;
    size_t ability_left()         const;
    std::list<int> const& ally_input_ids()  const;
    std::list<int> const& enemy_input_ids() const;

    void generate_heat(double);
    void lock_heat(bool const& f) { lock_heat_ = f; }
    bool is_controlled_by_AI()    const;
    int  jama_shoot_count()       const;
    //float haste_speedfunc(float orig_speed) const;

    virtual ~Player();

protected:
    Player(Input* input, int const&);
    pointer_type init();

    void normal_weapon_fx();
    void start_haste_effect();
    void remove_haste_effect();
    void normal_shot_delegate(view::pSprite&, HitCallback const&);
    void shot_delegate(view::pSprite&, HitCallback const&, wpointer_type);
    void repeating_shot_delegate(view::pSprite&, HitCallback const&, wpointer_type);
    void process_input();
    void heat_cooling();
    void end_overheat();
    int  delta_heat(double); //return 1 for overflow, 0 for nothing, -1 for underflow

protected:
    int       id_;
    int const changetime_;
    bool      changing_wep_;
    int       weplist_idx_;
    double    accumulated_heat_, cooling_speed_, heat_for_normal_shoot_, heat_for_haste_, heat_for_jama_shoot_;
    int       overheat_downtime_;
    bool      overheat_, hasting_;
    bool      lock_heat_; // 2012 note: setup a state to lock heat value.
    int       ability_kind_;
    int       jama_shoot_count_;

    Input*               input_;
	Weapon*              current_wep_;
	std::vector<Weapon*>          weplist_;
	std::vector<presenter::wpMap> map_list_;
	std::list<int>                ally_input_ids_, enemy_input_ids_;

	typedef std::tr1::function<void(wpointer_type const& player, presenter::wpMap const& self_map, presenter::wpMap const& enemy_map)>
	    AbilityCallback;
	std::deque< AbilityCallback > ability_queue_;

	std::tr1::function<void()> player_hit_event_;
	std::tr1::function<void(int, bool)> player_overheat_event_;
public:
	void player_hit_event(std::tr1::function<void()> const& ev) { player_hit_event_ = ev; }
	void player_overheat_event(std::tr1::function<void(int, bool)> const& ev) { player_overheat_event_ = ev; }
};

typedef std::tr1::shared_ptr<Player> pPlayer;
typedef std::tr1::weak_ptr<Player> wpPlayer;

} //ctrl
} //psc

#endif //_SHOOTING_CUBES_CTRL_PLAYER_
