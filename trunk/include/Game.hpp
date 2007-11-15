
/* 2007.11.9
  EVERYTHING KILLED. only skeleton framework (some game states) reserved,
  so in this file / class you can see everything a "Game" needs,
  but too many things require total over-haul.

  This file won't compile unless further changes made.

  These included file are going to be modified:
  1. Map.hpp
  2. Item.hpp
*/

#ifndef _SHOOTING_CUBE_GAME_HPP_
#define _SHOOTING_CUBE_GAME_HPP_

#include "Map.hpp"
#include "Factory.hpp"
#include "Item.hpp"
#include "Player.hpp"

#include <loki/Singleton.h>
#include <boost/random.hpp>
#include <ctime>
#include <vector>

#include "Debugger.hpp"

class Game{

public:
    Game(): dice_(engine_, boost::uniform_int<>(0, 16777215)),
        map1_(0,0), map2_(40,0), paused_(false), game_over_(0), need_suicide_(false), 
        checked_(0), stage_(0), item_creatable_(true), ignore_p2_(false),
		step_(0), game_mode_(0), time0_(0), retry_menu_(false),
    {
        // int chaos; // intent to use un-init-ed variable
        engine_.seed(static_cast<boost::uint32_t>(std::time(0)^std::clock()));

        //All the ugly accessment creation & deletion removed.
		
    }
    ~Game(){
        
        //All the ugly accessment creation & deletion removed.

    }

    int random() const{ return dice_(); }
    int random(int max) const{
        boost::variate_generator<boost::mt19937&, boost::uniform_int<> > dice = dice_;
        dice.distribution() = boost::uniform_int<>(0, max-1);
        return dice();
    }

	void make_garbage(Map*, int);

    void init(){
        map1_.init();
        map2_.init();

		ignore_p1_first_shot = true;
		player1_.set_trigger( true );   //TELL ME WHY ????????????????????????????????????????????????????????
		                                //Asynchronous Terror..............

    }
	void multi_init(){
		init();
		game_mode_ = 0;
	}

    void single_init()
    {
        map2_.ms().dropping_creatable = false;
        map2_.ms().starting_line = 0;
		game_mode_ = 1;
		ignore_p2_ = true;
		item_creatable_ = false;

        init();
    }

    void puzzle_init(int stage_num = 1)
    {
        map1_.ms().dropping_creatable = false;
        map1_.ms().starting_line = 0;
        map2_.ms().dropping_creatable = false;
        map2_.ms().starting_line = 0;
        stage_ = stage_num;
		game_mode_ = 2;
		ignore_p2_ = true;

		game_over_ = 0;
		checked_ = 0;
        item_creatable_ = false;
		retry_menu_ = false;

        player1_.disable_all_wep_reloadability();

        //All the hard-coded stage data removed to a temporary txt file.

		player1_.weapon()->reset();
		player1_.weapon()->ammo(1);

		//Need to load puzzle stage hint here.

        init();
    }

	//this will only be used temporarily..... ugly
	int cycle();

	MapSetting const& ms() const;
	void itembox_creation();

	void player1_start_firing();
	void player2_start_firing();
	void player1_firing_continuously();
	void player2_firing_continuously();
	void player1_confirm_hit_square(int x, int y);
	void player2_confirm_hit_square(int x, int y);

	void light_player1_icon(int guntype);
	void light_player2_icon(int guntype);

	void icon_cycle();

	void set_all_player_buttons();

	//add just for now.
	Map & map1();
	Map & map2();

    // added
    void pause();
    bool is_paused() const{ return paused_; }
	bool is_retry_on() const{ return retry_menu_; }
	int  game_mode() const{ return game_mode_; }

	static int PUZZLE_RETRY;

	int endgame_sequence();

    void game_over(Map* who_lose);
    void no_warning(Map* who_safe_now);
    void yell_warning(Map* who_dangerous_now, int countdown);
    void graceful_suicide(){ need_suicide_ = true; }

private:
    void update_info();
    void produce_map(int array[11][6]);
	int  gameover_proc();
	void puzzle_stage_check();


private:
    boost::mt19937 engine_;
    mutable boost::variate_generator< boost::mt19937&, boost::uniform_int<> > dice_;
	Map map1_;
	Map map2_;
	Player player1_;
	Player player2_;

	//added
	Item itembox_;
	int step_;

    // Item: Game state control and management
    // Item: 3D Scene Manager Accessibility

	int game_over_delay_;
	time_t time0_;

    // added
    bool paused_;
	bool retry_menu_;

    int game_over_;
    bool need_suicide_;
    
    // Item: Stage Message ( like game over or pause or retry or something )
    // Item: 1p & 2p specific Message ( like chain or warning message )
    // Item: 1p & 2p Score Indicator
    // Item: 1p & 2p Bullet Indicator
	// Item: 1p & 2p Weapon List / Indicator

    // Item: Top score menu

    // added by puzzle
    int checked_;
    int stage_;
    bool item_creatable_;
	bool ignore_p1_first_shot;
	bool ignore_p2_;
	int game_mode_;
};

typedef Loki::SingletonHolder<Game, Loki::CreateUsingNew, Loki::DeletableSingleton> SG;

#endif


