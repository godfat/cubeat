
/*2007.11.9 
  cleaned up things which were coupled with VtMovieClip
  
  These included file is going to be heavily modified or deleted:
  1. VtMovieClip.hpp
  2. MovieClip.hpp
*/         

#ifndef _SHOOTING_CUBE_SQUARE_HPP_
#define _SHOOTING_CUBE_SQUARE_HPP_

#include "VtMovieClip.hpp"

#include "PooledObject.hpp" // parent
#include "MovieClip.hpp"    // contain value
#include "Color.hpp"        // contain value

#ifndef NDEBUG
#include <iostream>
#include <cassert>
#endif

class Map;
class State;
class Waiting;
class Dropping;
class Dying;
class Exploding;
class Chain;

class Square: public PooledObject
{
public:
    Square();

    #ifndef NDEBUG
    friend std::ostream& operator<<(std::ostream& lhs, Square const& rhs);
    #endif

    // begin used by Map
    int x() const{ assert(inited_); return x_; }
    int y() const{ assert(inited_); return y_; }
    // end

    // begin only used by Map::insert / fill_preview_queue
    Square* init(Map* const map, int x, int y);
	Square* init(Map* const map, int x, int y, int color_id);
	Square* init2(Map* const map, int x, int y);
    // end

    // begin only used by Map::cycle
    bool cycle_and_die();
    void refresh(){ cycled_ = false; }
    // end

    // begin only used by Map::next_state
    void drop_a_block(){ ++y_; }
    Waiting* is_waiting();
    Dropping* is_dropping();
    void lose_chain();
    // end

    // begin only used by Map::square_can_lose_chain,
    //   which is used by Map::next_state
    Dying const* is_dying() const;
    // end

    // begin only used by OneFading::get_the_chain,
    //   which is used by OneFading::fade
    Chain*& chain(){ return chain_belonged_to_; }
    // end

    // begin only used by Map::squares_belong_to_the_chain_from_bottom_to_top
    Chain const* chain() const{ return chain_belonged_to_; }
    // end


    // begin only used by OneFading::fade
    void go_dying();
    void cycled(){ cycled_ = true; }
    // end

	// begin only used by State::map
    Map& map(){ assert(inited_); return *map_; }
    Map const& map() const{ assert(inited_); return *map_; }
    // end

    // begin only used by Dropping::cycle
    void drop_a_frame();
    bool is_not_reached() const{ assert(inited_); return body_.y() < this->real_y(); }
    void addjust(){ assert(inited_); body_.y() = this->real_y(); }
    // end

    // begin only used by Map.cpp => local function check_not_continue
    friend inline bool operator!=(Square const& lhs, Square const& rhs){ return lhs.color_ != rhs.color_; }
    // end


	// begin only used by Map::insert_garbage,
	//   which is used by Map::cycle_creation_garbage
	void be_garbage();
	// end

	bool is_garbage() { return is_garbage_; }
	bool has_grounded() { return has_grounded_; } 
	void set_grounded() { has_grounded_ = true; }

	//added a ugly thing:
	bool is_this_mine( /* Something* */ );
	void owner_hit_me(int dmg);
	void enemy_hit_me(int dmg);

    void be_broken();
	void restore();

	//graphical issues, call body to resolve
	void set_alpha(int alpha)     { body_.set_alpha(alpha); }
	void hide_outline()           { body_.hide_outline(); }
	void show_outline()           { body_.show_outline(); }
	void explosion_setup()        { body_.explosion_setup(); }
	void explosion_loop(int step) { body_.explosion_loop( step ); }
	void explosion_cleanup()      { body_.explosion_cleanup(); }
	//end

	//added
	Exploding const* is_exploding() const;
	void go_exploding();
	void go_exploding(int step);

	int& lose_chain_delay() { return lose_chain_delay_; }
	int const& lose_chain_delay() const{ return lose_chain_delay_; }

private:
    virtual void do_init();
    virtual void do_suicide();
    void setup(Map* const map, int x, int y);
    double real_y() const;

private:
    Map* map_;
    double x_, y_;
    Color color_;
    MovieClip<VtMovieClip> body_;
    State* state_;
    Chain* chain_belonged_to_;
    bool inited_;
    bool cycled_;

	bool is_broken_;
	bool is_garbage_;
	int  hp_;
	bool has_grounded_;

	int lose_chain_delay_;
};

#endif
