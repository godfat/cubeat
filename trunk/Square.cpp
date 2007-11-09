
/*2007.11.9 
  cleaned up things which were coupled with VtMovieClip

  This file won't compile unless further changed made.
  
  These included file are going to be heavily modified or deleted:
  1. VtMovieClip.hpp
  2. Sound.hpp
  3. Map.hpp
*/         

#include "Square.hpp"
#include "Map.hpp"
#include "Factory.hpp"
#include "State.hpp"
#include "Dying.hpp"
#include "Exploding.hpp"

#include "Sound.hpp"

#include <boost/bind.hpp> // for MovieClip callback

#ifndef NDEBUG
#include <iostream>
#endif

#include <cassert>

Square::Square(): inited_(false)
{}

#ifndef NDEBUG
std::ostream& operator<<(std::ostream& lhs, Square const& rhs){
    lhs << "Square cycle at: (" << rhs.x_ << ", " << rhs.y_ << "); state: " << typeid(*rhs.state_).name()
        << "; location at: " << rhs.body_ << "\n";
    return lhs;
}
#endif

void Square::setup(Map* const map, int x, int y){
    map_ = map; x_ = x; y_ = y;
    body_.init(x_, y_, map_->ms().size, &color_, map_->get_entity() );
    inited_ = true;

	is_broken_ = false;
	is_garbage_ = false;
	hp_ = 1;
	has_grounded_ = false;
	lose_chain_delay_ = 0;

    #ifndef NDEBUG
    std::clog << "Square init at: (" << x_ << ", " << y_ << ")\n";
    #endif
}

// only used by Map::insert
Square* Square::init(Map* const map, int x, int y){
    color_ = map->random_color();
    setup(map, x, y);
    return this;
}

Square* Square::init2(Map* const map, int x, int y){
	setup(map, x, y);
	return this;
}

// used by Map's testcase or puzzle solving or fill_preview_queue
Square* Square::init(Map* const map, int x, int y, int color_id){
    color_ = map->id_color(color_id);
    setup(map, x, y);
    return this;
}

// only used by Map::cycle
bool Square::cycle_and_die(){
    assert(inited_);
    body_.cycle();
    if( cycled_ )
        return false; // i am alive though killed, think of DYING NOW

    #ifndef NDEBUG
    std::clog << *this;
    #endif
    State* next_state = state_->cycle();
    if(next_state){
        if(next_state != state_){
            state_->suicide();
            state_ = next_state;
        }
        return false; // i am alive
    }
    else{
        this->suicide();
        return true; // i am dead
    }
}

//added:
void Square::owner_hit_me(int dmg)
{
	if( is_dying() || is_exploding() ) return;
	if( is_broken_ == true ) {
		restore();
		Sound::i().broken_cube();
	}
	else {
		hp_ -= dmg;
		if( is_garbage_ == true ) {
			if( hp_ == 2 )      go_exploding(3);
			else if( hp_ == 1 ) go_exploding(6);
			Sound::i().rock_cube();
		}
		else Sound::i().normal_cube();
	}

	if( hp_ <= 0 ) {
		go_exploding();
		cycled();
	}
}

//added:
void Square::enemy_hit_me(int dmg)
{
	if( is_dying() || is_exploding() ) return;
	if( is_garbage_ == true ) {
		Sound::i().rock_cube();
		return;
	}
	else if( is_broken_ == true ) {
		hp_ -= dmg;
		Sound::i().broken_cube();
	}
	else {
		be_broken(); 
		Sound::i().normal_cube();
	}

	if( hp_ <= 0 ) {
		go_exploding();
		cycled();
	}
}

// begin only used by Map::next_state
Waiting* Square::is_waiting(){ assert(inited_); return dynamic_cast<Waiting*>(state_); }
Dropping* Square::is_dropping(){ assert(inited_); return dynamic_cast<Dropping*>(state_); }
Dying const* Square::is_dying() const{ return dynamic_cast<Dying*>(state_); }
Exploding const* Square::is_exploding() const{ return dynamic_cast<Exploding*>(state_); }

void Square::lose_chain() {
	if(chain_belonged_to_) {
		chain_belonged_to_->dec_one_owner();
		chain_belonged_to_ = NULL;
	}
}
// end


// only used by OneFading::fade
void Square::go_dying()
{ 
	state_ = SF::Instance().Create<Dying>()->init(this); 
	state_->init2();
}

void Square::go_exploding()
{
	state_ = SF::Instance().Create<Exploding>()->init(this);
	state_->init2();
}

void Square::go_exploding(int step)
{
	body_.explosion_setup();
	body_.explosion_loop(step);
}

// only used by Dropping::cycle
void Square::drop_a_frame() 
{   
	assert(inited_); 
	if( is_garbage() || has_grounded() ) 
	    body_.y() += 10.0;  //a very fast fixed speed, NOTE: maybe ms().maxspeed will do
	else {
		if( map_->is_speedup() ) 
		    body_.y() += 10.0;  //a very fast fixed speed, NOTE: maybe ms().maxspeed will do
		else
			body_.y() += map_->ms().speed;
	}
}


// private below
double Square::real_y() const{ return map_->ms().size * y_; }

void Square::do_init(){
    state_ = SF::Instance().Create<Waiting>()->init(this);
    chain_belonged_to_ = NULL;
}

void Square::do_suicide(){
    #ifndef NDEBUG
    std::clog << "Square dying at: (" << x_ << ", " << y_ << ")\n";
    #endif
    inited_ = false;
    lose_chain();  //IMPORTANT: I forgot this...
    // type_->suicide();
    state_->suicide();
	body_.hide();
    #ifndef NDEBUG
    std::clog << "Square died at: (" << x_ << ", " << y_ << ")\n";
    #endif
}

//added
void Square::be_garbage()
{
	is_garbage_ = true;
	hp_ = 3;
    body_.be_garbage();
    color_ = map_->garbage_color();
	show_outline();
}

void Square::be_broken()
{
	is_broken_ = true;
	body_.be_broken();
	color_ = map_->garbage_color();
}

void Square::restore()
{
    is_broken_ = false;
	color_ = map_->random_color();
	body_.update_color( &color_ );
	if( Waiting* st = is_waiting() )
		if( !st->need_check() )
		    st->uncheck();
}

bool Square::is_this_mine( /* Something* pick */ )
{
	return body_.is_this_mine( /*pick*/ );
}
