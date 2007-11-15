
/*2007.11.9 
  cleaned up things which were coupled with VtMovieClip
  
  These included file is going to be heavily modified or deleted:
  1. VtMovieClip.hpp
  2. MovieClip.hpp
*/         

#ifndef _SHOOTING_CUBE_ITEM_HPP_
#define _SHOOTING_CUBE_ITEM_HPP_

#include "VtMovieClip.hpp"

#include "PooledObject.hpp" // parent
#include "MovieClip.hpp" // contain value
#include "Color.hpp"     // contain value

#ifndef NDEBUG
#include <iostream>
#include <cassert>
#endif

class Game;
// class Type;

class Item
{
public:
    Item();

    #ifndef NDEBUG
    // friend std::ostream& operator<<(std::ostream& lhs, Square const& rhs);
    #endif

    // begin used by Game
    int x() const{ assert(inited_); return x_; }
    int y() const{ assert(inited_); return y_; }
	int z() const{ assert(inited_); return z_; }
    // end

    // begin only used by Game
    Item* init(Game* const game, int dir);
	void suicide() { do_suicide(); }
    // end

    // begin only used by Game::cycle
    bool cycle_and_die();
    // end

    // begin only used by Game::??
    bool is_moving() const;
    // end

    // begin only used by Game::??
    bool is_dying() const;
	bool is_dead()  const;
    // end

    // begin only used by Game::??
    void go_dying();
    // end

    // begin only used by self
    void move_a_frame();
    bool reached_dest() const;
    // end
    
	//added a ugly thing:
	bool is_this_mine( /* Something* */ );
	void i_am_hit();

	int type() const { return item_type_; }

	void hide_outline() { body_.hide_outline(); }
	void show_outline() { body_.show_outline(); }

private:
    void do_init();
    void do_suicide();
    void setup(Game* const game, int x, int y, int z);
    //void onClick(){
        // type_->onClick() ? this->suicide();
    //}

private:
    Game* game_;
    int x_, y_, z_;
    Color color_;
    MovieClip<VtMovieClip> body_;
	bool inited_;
	int state_;    //moving = 2    dying = 1   died = 0
	int dir_;      // 1 for left to right, otherwise right to left
	int from_;
	int to_;
	int speed_;

	int item_type_;
};

#endif //endif