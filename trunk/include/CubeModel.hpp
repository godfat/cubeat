
/*2007.11.9 
  cleaned up things which were coupled with VtMovieClip
  
  These included file is going to be heavily modified or deleted:
  1. VtMovieClip.hpp
  2. MovieClip.hpp
*/         

#ifndef _SHOOTING_CUBE_CUBE_MODEL_HPP_
#define _SHOOTING_CUBE_CUBE_MODEL_HPP_

#include "Color.hpp"        // contain value

class Map;
class State;
class Waiting;
class Dropping;
class Dying;
class Exploding;
class Chain;

class CubeModel{
public:
    CubeModel();

    // begin used by Map
    int x() const{ return x_; }
    int y() const{ return y_; }
    // end

    // begin only used by Map::insert / fill_preview_queue
    CubeModel* init(Map* const map, int x, int y);
	CubeModel* init(Map* const map, int x, int y, int color_id);
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
    Map& map(){ return *map_; }
    Map const& map() const{ return *map_; }
    // end

    // begin only used by Map.cpp => local function check_not_continue
    friend inline bool operator!=(Square const& lhs, Square const& rhs){ return lhs.color_ != rhs.color_; }
    // end

	// begin only used by Map::insert_garbage,
	//   which is used by Map::cycle_creation_garbage
	void be_garbage();
	// end

	bool is_garbage() const{ return is_garbage_; }
    void be_broken();
	void restore();

	//added
	Exploding const* is_exploding() const;
	void go_exploding();

	int& lose_chain_delay() { return lose_chain_delay_; }
	int const& lose_chain_delay() const{ return lose_chain_delay_; }

private:
    Map* map_;
    int x_, y_;
    Color color_;
    State* state_;
    Chain* chain_belonged_to_;
    bool inited_;
    bool cycled_;

	bool is_broken_;
	bool is_garbage_;
	int  hp_;

	int lose_chain_delay_;
};

#endif
