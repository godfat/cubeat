
/*2007.11.9 
  cleaned up things which were coupled with VtMovieClip

  This file won't compile unless further changed made.
  
  These included file is going to be heavily modified or deleted:
  1. VtMovieClip.hpp
  2. Sound.hpp
  3. Game.hpp
  4. Item.hpp
*/      

#include "Game.hpp"
#include "Factory.hpp"
#include "Weapon.hpp"
#include "Item.hpp"

#include <boost/bind.hpp> // for MovieClip callback

#ifndef NDEBUG
#include <iostream>
#endif

#include <cassert>

Item::Item(): inited_(false), state_(0)
{}

void Item::setup(Game* const game, int x, int y, int z){
    game_ = game; x_ = x; y_ = y; z_ = z;
	body_.init(x_, y_, z_, game_->ms().size, &color_, game_->map1().get_entity() );
    inited_ = true;

	item_type_ =  game_->random(4);
	switch ( item_type_ ) {
		case 0: /* MovieClip load img1 as texture */ break;
		case 1: /* MovieClip load img2 as texture */ break;
		case 2: /* MovieClip load img3 as texture */ break;
		case 3: /* MovieClip load img4 as texture */ break;
	}

	Debugger::i().trace( "Itembox Inited" );
}

// only used by Game
Item* Item::init(Game* const game, int dir)
{
	Debugger::i().trace( "Itembox: before do init" );

	do_init();
    color_ = Color(128, 128, 128);
	dir_ = dir;

	Debugger::i().trace( "Itembox: before decide speed, from, to" );
	
	if( dir_ == 1 ) {
		speed_ = (game->ms().speed + 1);  //a little faster than normal cubes
		from_ = -3;
		to_ = 18;
	}
	else {
		speed_ = -(game->ms().speed + 1);
		from_ = 18;
		to_ = -3;
	}

	Debugger::i().trace( "Itembox: before itembox setup" );
	//we make the Item's position and direction random here

	int y = game->random(5)+2;
	int z = game->random(2) ? - (game->random(8)+1) : (game->random(5)+1);         
	//it's a flaw in the procedure. we should use "game" here, not "game_"
	//"game_" is only useable after setup();

	setup(game, from_, y, z );             
    return this;
}

bool Item::cycle_and_die(){
    assert(inited_);
    body_.cycle();
    
    #ifndef NDEBUG
    //std::clog << *this;
    #endif
    
	if( state_ != 0 ) {
		if( is_moving() ) {
			move_a_frame();
			if( reached_dest() )   
				go_dying();
		}
		else if( is_dying() )  
			suicide();  /* do something */ 

		return false;
    }
    else {
        do_suicide();
        return true; // i am dead
    }
}
    
bool Item::is_moving() const { return state_ == 2; }
bool Item::is_dying() const { return state_ == 1; }
bool Item::is_dead()  const { return state_ == 0; }
void Item::go_dying() { state_ = 1; }
    
void Item::move_a_frame() { body_.x() += speed_*3; }
bool Item::reached_dest() const 
{ 
	assert(inited_); 

	if( dir_ == 1 ) {
		return body_.x() >= to_ * game_->ms().size;
	}
	else {
		return body_.x() <= to_ * game_->ms().size;
	} 
}

bool Item::is_this_mine( /* Something* pick */)
{
	return body_.is_this_mine( /* pick */ );
}

void Item::i_am_hit() 
{
    go_dying();
}

void Item::do_init()
{
    state_ = 2;
}

void Item::do_suicide()
{
    #ifndef NDEBUG
    std::clog << "Square dying at: (" << x_ << ", " << y_ << ")\n";
    #endif
    inited_ = false;
    state_ = 0;
	body_.hide();
    #ifndef NDEBUG
    std::clog << "Square died at: (" << x_ << ", " << y_ << ")\n";
    #endif
}
