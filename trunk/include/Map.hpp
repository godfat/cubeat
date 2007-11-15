
/*2007.11.9 
  cleaned up minor things, and those which were coupled with Virtools SDK,
  Fortunately there are not many.

  This file won't compile unless further changes made.
*/  

#ifndef _SHOOTING_CUBE_MAP_HPP_
#define _SHOOTING_CUBE_MAP_HPP_

#include "Color.hpp"
#include "Factory.hpp"

#include <vector>
#include <set>

#include <ctime>
#include <cmath>

namespace{

template <typename T, template <class, class> class Container = std::vector,
                      template <class> class Allocator = std::allocator>
class vector_2d{
public:
    typedef T element_type;
    typedef Container< T, Allocator<T> > container_type;
    typedef typename container_type::iterator iterator;
    typedef typename container_type::const_iterator const_iterator;
    typedef typename container_type::reverse_iterator reverse_iterator;
    typedef typename container_type::const_reverse_iterator const_reverse_iterator;
    typedef typename container_type::size_type size_type;

public:
    vector_2d(size_type width, size_type height):
        width_(width), height_(height),
        size_(width*height), data_(size_, 0)
    {}

    size_type width() const{ return width_; }
    size_type height() const{ return height_; }

    T& at(size_type x, size_type y){ return data_.at(width_*y+x); }
    T const& at(size_type x, size_type y) const{ return data_.at(width_*y+x); }

    iterator begin(){ return data_.begin(); }
    const_iterator begin() const{ return data_.begin(); }
    iterator end(){ return data_.end(); }
    const_iterator end() const{ return data_.end(); }
    reverse_iterator rbegin(){ return data_.rbegin(); }
    const_reverse_iterator rbegin() const{ return data_.rbegin(); }
    reverse_iterator rend(){ return data_.rend(); }
    const_reverse_iterator rend() const{ return data_.rend(); }

private:
    size_type width_;
    size_type height_;
    size_type size_;
    container_type data_;
};

class Checker{
    friend bool is_this_good_to_drink(vector_2d<int> const&, int, int, int);
    Checker(vector_2d<int> const& square_colors, int x, int y, int chain_amounts):
    square_colors_(square_colors), x_(x), y_(y), chain_amounts_(chain_amounts),
    tested_color_(square_colors_.at(x_, y_))
    {}

    bool no_left() const{
        if( x_ < chain_amounts_-1 ) return true;
        for(size_type i=1; i<chain_amounts_-1; ++i)
            if( tested_color_ != square_colors_.at(x_-i, y_) )
                return true;
        return false;
    }
    bool no_right() const{
        if( x_ > square_colors_.width() - chain_amounts_ ) return true;
        for(size_type i=1; i<chain_amounts_-1; ++i)
            if( tested_color_ != square_colors_.at(x_+i, y_) )
                return true;
        return false;
    }
    bool no_up() const{
        if( y_ > square_colors_.height() - chain_amounts_ ) return true;
        for(size_type i=1; i<chain_amounts_-1; ++i)
            if( tested_color_ != square_colors_.at(x_, y_+i) )
                return true;
        return false;
    }
    bool no_down() const{
        if( y_ < chain_amounts_-1 ) return true;
        for(size_type i=1; i<chain_amounts_-1; ++i)
            if( tested_color_ != square_colors_.at(x_, y_-i) )
                return true;
        return false;
    }

private:
    vector_2d<int> const& square_colors_;
    typedef vector_2d<int>::size_type size_type;
    size_type const x_, y_;
    size_type const chain_amounts_;
    int const tested_color_;
};

bool is_this_good_to_drink(vector_2d<int> const& square_colors, int x, int y, int chain_amounts){
    Checker c(square_colors, x, y, chain_amounts);
    return c.no_left() && c.no_right() && c.no_up() && c.no_down();
}

bool is_gooood(vector_2d<int>& square_colors, int chain_amounts){
    std::random_shuffle(square_colors.begin(), square_colors.end());
    for(int y=0, yend=square_colors.height(); y!=yend; ++y)
        for(int x=0, xend=square_colors.width(); x!=xend; ++x)
            if(is_this_good_to_drink(square_colors, x, y, chain_amounts));
            else return false;
    return true;
}

} // local namespace



struct MapSetting{
    MapSetting():
        width(6), height(11), size(50),
        speed(0.75), frequency(100), decay(22), delay(3),
        color_amounts(4), chain_amounts(3), damage_factor(1.00), 
		starting_line(4), dropping_creatable(true)
    {}
    int width, height, size;
	int frequency, decay, delay;
	double speed;
	double damage_factor;
    int color_amounts, chain_amounts;
	int starting_line;
    bool dropping_creatable;
};

class Square;
class State;
class Chain;
class OneFading;

class Map{
public:
    typedef vector_2d<Square*> container_type;
    typedef std::set<Square*> slist_type;

public:
	//signature changed
    Map(float xoff, float yoff):
	  data_(ms().width, ms().height),
	  xoffset_(xoff),  //added
	  yoffset_(yoff),   //added
      score_(0)
    {
		//We need to load warning bars here, and there might still be other
        //graphic elements for "map" in the future, so better be careful.

		garbage_left_ = 0;
		garbage_step_ = 0;
		creation_step_ = 0;
        speed_up_step_ = time(0);
		n_of_newcomers_ = 0;
	}
    Map& init(){
		vector_2d<int> square_colors(ms().width, ms().starting_line);
        {
            int const one_color_amounts = std::ceil(static_cast<double>(ms().width)*ms().starting_line/ms().color_amounts);
            int count = 0;
            for(vector_2d<int>::iterator i=square_colors.begin(), iend=square_colors.end();
                i!=iend; ++i, ++count)
            {
                *i = count/one_color_amounts;
            }
        }
        while(!is_gooood(square_colors, ms().chain_amounts));
        {
            int count = 0;
            for(vector_2d<int>::iterator i=square_colors.begin(), iend=square_colors.end();
                i!=iend; ++i, ++count)
            {
                insert(SF::Instance().Create<Square>(), count%ms().width, ms().height-1-count/ms().width, *i);
            }
        }

		fill_preview_queue(); //added

        return *this;
    }
    ~Map();

    // begin only used by Game
    void insert(Square* that, int x, int y);
	void insert(Square* that, int x, int y, int color_id);   //added
    void cycle();
    // end

    // only used by Game::make_garbage
    void add_garbage(int amounts) { garbage_left_ += amounts; }
    // end

    // begin only used by Square::init
    Color random_color() const;
	Color id_color(int) const;
	Color garbage_color() const;           // added
    // end

    // begin used by Dropping::cycle, Waiting::cycle
    State* next_state(Square*);
    // end

    // begin used by Dropping::cycle, Square::real_y, etc...
    MapSetting const& ms() const{ return map_setting_; }
    MapSetting& ms(){ return map_setting_; }
    // end

    // begin only used by OneFading::fade
    void squares_belong_to_the_chain_from_bottom_to_top(Square*, Chain*);
    // end

    // begin only used by OneFading::get_the_chain
    //   which is used by OneFading::fade
    void push_chain(Chain* chain){ chains_.push_back(chain); }
    // end

	//added
	CK3dEntity* get_entity() { return ent_; }

	//added ugly function
    bool owner_hit_you( /* Something* */, int );
	void enemy_hit_you( /* Something* */, int );
    int score() const{ return score_; }

    // added by puzzle
	bool all_empty();
	bool have_alive_chain();
	bool no_one_dropping();
	bool every_one_is_checked();

	bool is_speedup() { return speedup_; }
	void speedup(bool in) { speedup_ = in; } 

	int& n_of_newcomers() { return n_of_newcomers_; }

private:
    void make_row(Square* that, slist_type&, slist_type&);
    void make_column(Square* that, slist_type&, slist_type&);
    void make_slist(slist_type&, slist_type&, slist_type&);
    OneFading* make_OneFading(Square*);
    bool is_below_empty(Square*) const;
    void cycle_squares();
    void update_chains();
    void refresh_squares();
    bool square_can_lose_chain(Square const*) const;
    void insert_garbage(Square* that, int x, int y);        //added

	//added
    void cycle_creation();
    void cycle_creation_garbage();
	bool check_column_full();

	void cycle_warning();

	//added create preview queue ralated
	void fill_preview_queue();
	void fill1(std::vector<int>&, int&);
	void fill2(std::vector<int>&, int&);

	void load_warning_strips() {
		//removed for now.
	}

private:

    MapSetting map_setting_;
    container_type data_;

    // someday make it to be std::list
    std::vector<Chain*> chains_;

	// added: preview blocks and a newcomer blocks' reference
	std::vector<Square*> preview_;
	int n_of_newcomers_;

	//added
    float xoffset_;
    float yoffset_;
    /* Something* ent_; */

    int garbage_left_;
    int garbage_step_;
    int creation_step_;

    // added
    int score_;
    time_t lose_warning_step_;
    time_t speed_up_step_;
	time_t freq_up_step_;

	//added another ugly thing
	/* std::vector<CK3dEntity*> warning_strip_; */
	int counting_num;
	bool speedup_;
	bool garbage_switch_;
};

#endif
