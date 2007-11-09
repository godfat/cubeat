
/* 2007.11.9
  EVERYTHING KILLED. only skeleton framework (some game states) reserved,
  so in this file / class you can see everything a "Game" needs,
  but too many things require total over-haul.

  This file won't compile unless further changes made.

  These included file is going to be heavily modified or deleted:
  1. Game.hpp
  2. Input.hpp
  3. Sound.hpp
*/

#include "Game.hpp"
#include "Input.hpp"
#include "Sound.hpp"

#include <fstream>
#include <iomanip>

int Game::PUZZLE_RETRY = 0;

MapSetting const& Game::ms() const
{
	return map1_.ms();
}

//for inline's sake.. move it here
void Game::make_garbage(Map* map, int amounts) {
    if( map != &map1_ ) {
		map1_.add_garbage(amounts);
    } 
	else {
		map2_.add_garbage(amounts);
    }
}

void Game::pause(){
    paused_ = !paused_;
}

void Game::game_over(Map* who_lose)
{
    //Show won & lose stage messages here
    //And don't forget to update game state (game_over_ or something)
}

void Game::no_warning(Map* who_safe_now){
    /* clear warning */
}

void Game::yell_warning(Map* who_dangerous_now, int countdown){
	/* blah */
}


void Game::update_info()
{
    //Update everything you need to update here:
    //Scores, Weapon icons, bullet numbers, etc.
}

void Game::icon_cycle()
{
	//light or dim the icons (bullet.. weapon.. all the hell).
}

int Game::endgame_sequence()
{
	if( time0_ == 0 ) {
		if( retry_menu_ == true ) game_over_delay_ = 0;     
		else {
			if( game_mode_ == 2 ) game_over_delay_ = 180;
			else                  game_over_delay_ = 720;
		}
        time0_ = time(0);
		//hide the warning
    }

	if( game_over_delay_ >= 0 ) {
		--game_over_delay_;
		if( game_over_delay_ == 541 ) {    //start showing top scores here
            //hide the won text & the score texts
			//You know.. just read and write the top score savefiles and show them.
		}
        else if( game_over_delay_ <= 540 ) {
            //Show the score texts accordingly. this part is rather easy.( and ugly )
		}
		return 0;
	}
	else {
		time0_ = 0;
		return game_over_;
	}
}


int Game::cycle() 
{
    update_info();
	icon_cycle();
	set_all_player_buttons();
	player1_.cycle();
	if( ignore_p2_ == false ) player2_.cycle();

    if( need_suicide_ ) return 2;
    if( paused_ )       return 0; // 0 should be no one won
	if( game_over_ )    return endgame_sequence(); 
	
	/*********** PLAYER INPUT RELATED ************/
	if( player1_.triggered() ) player1_firing_continuously();
	
	if( Input::i().D1() == TRUE && player1_.triggered() == false ) {
		if ( ignore_p1_first_shot == true )
			ignore_p1_first_shot = false;
		else {
			if( player1_.weapon()->ammo() == 0 ) {
				player1_.set_trigger( true );
				Sound::i().out_of_ammo();
			}
			player1_start_firing();
		}
	}

	/* Player 2 trigger and firing functions are ignored for now,
       because player 1 and player 2 functions are actually identical */

	map1_.speedup( Input::i().Down1() ? true : false );    //added speedup control
	map2_.speedup( Input::i().Down2() ? true : false ); 

    /***************** END PLAYER INPUT RELATED*******************/

    if(item_creatable_){
	    itembox_creation();
	    if( !itembox_.is_dead() ) {
		    itembox_.cycle_and_die();
	    }
    }
	//map cycles
	map1_.cycle();
	map2_.cycle();

    if(stage_ == 0) // not in puzzle mode
        return 0;

    // in puzzle stage mode... if you can arrive here   
	puzzle_stage_check();
    return 0;
}

void Game::puzzle_stage_check() 
{
    if( map1_.all_empty() ) {
		//The Stage is Cleared in One Shot (or shots given).
	}

	if( map1_.every_one_is_checked() && player1_.weapon_empty() ) {
		checked_++;
		if( checked_ > 5 ) {  //Check delay (?)
            
            //FAILED!!!!! (stage not cleared)

			if( PUZZLE_RETRY > 0 ) { 
				//not lost so fast..... give them some ugly retry ...... 
			}
	    }
	}
	else checked_ = 0;
}

/* player 1 specific functions */
void Game::player1_start_firing()
{
	if( player1_.is_changing_wep() ) return;
	if( player1_.weapon()->can_fire() == false ) return;

    player1_.set_trigger( true );
	player1_.weapon()->fire();

	if( dynamic_cast<Rocket*>( player1_.weapon() ) ) {
		for( int i = -1; i <= 1; ++i ) {        //iterate through y
			for( int j = -1; j <= 1; ++j ) {    //iterate through x
				player1_confirm_hit_square( (int)Input::i().pos1().x + j * ms().size * 0.9f, 
					                        (int)Input::i().pos1().y + i * ms().size * 0.9f );
			}
		}
	}
	else {
		player1_confirm_hit_square( (int)Input::i().pos1().x , (int)Input::i().pos1().y );
	}		
}

void Game::player1_firing_continuously()
{
	if( player1_.weapon()->can_fire_repeatedly() == false ) return; 
	if( player1_.weapon()->can_fire() == false ) return;
	
	player1_.weapon()->fire();
	
	player1_confirm_hit_square( (int)Input::i().pos1().x , (int)Input::i().pos1().y );
}

void Game::player1_confirm_hit_square(int x, int y)
{
	/* Ok, we need to redo all the player picking things.
       Need to test for itembox or things in map1 / map 2. */
}

/* end of player1 specific functions */

/* Player 2 trigger and firing functions are ignored for now,
   because player 1 and player 2 functions are actually identical */

void Game::itembox_creation()
{
	if( ++step_ > map1_.ms().frequency * 5 ){
		Debugger::i().trace( "Itembox creation" );
		step_ = 0;
		if( !itembox_.is_dead() ) itembox_.suicide();
		Debugger::i().trace( "Itembox init process start" );
		itembox_.init( this, random(2) );	
		itembox_.show_outline();

		Sound::i().sp_event();
    }
}

Map & Game::map1() { return map1_; }
Map & Game::map2() { return map2_; }

void Game::set_all_player_buttons()
{
	/* needs overhual */
}

void Game::produce_map(int array[11][6])
{
	Debugger::i().trace(" reach 1 ");
	for( int i = 10; i >= 0; i-- ) {
		for( int j = 5; j >= 0 ; j-- ) {
			if( array[i][j] != 9 ) {
				map1_.insert( SF::Instance().Create<Square>(), j, i, array[i][j] );
			}
		}
	}
}