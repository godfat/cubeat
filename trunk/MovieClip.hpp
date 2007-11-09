
/*2007.11.9 
  cleaned up things which were coupled with Virtools SDK.

  This file won't compile unless further changes made.
  Filename and classes contained are going to be renamed.
  
  These included file is going to be heavily modified or deleted:
  1. VtMovieClip.hpp
*/         

#ifndef _SHOOTING_CUBE_MOVIE_CLIP_HPP_
#define _SHOOTING_CUBE_MOVIE_CLIP_HPP_

#include "VtMovieClip.hpp"

#include "Location.hpp"
#include "Color.hpp"

#include <boost/function.hpp> // used for callback

class CK3dEntity;

template <class Imp>
class MovieClip{
public:
	MovieClip()
    {}
	void init(int x, int y, int size, Color const* color, CK3dEntity* parent_ent){
        loc_.x = x*size; loc_.y = y*size; size_ = size; color_ = color;
        imp_.init(loc_, *color_, size_, parent_ent );
    }

	//overloaded for Item initialization
	void init(int x, int y, int z, int size, Color const* color, CK3dEntity* parent_ent){
		loc_.x = x*size; loc_.y = y*size; loc_.z = z*size; size_ = size; color_ = color;
        imp_.init(loc_, *color_, size_, parent_ent );
    }

    double x()const{ return loc_.x; }
    double y()const{ return loc_.y; }
    double z()const{ return loc_.z; }
    double& x(){ return loc_.x; }
    double& y(){ return loc_.y; }
    double& z(){ return loc_.z; }
    Color const& color() const{ return *color_; }
    void cycle(){
        imp_.cycle(loc_, *color_);
    }
	void hide(){
		imp_.hide();
	}

	//added
	void be_garbage() {
		imp_.be_garbage();
	}

	void be_broken() {
		imp_.be_broken();
	}

	void update_color( Color const* color ) {
		color_ = color;
		imp_.update_color( *color_ );
	}

	bool is_this_mine( /* Something* pick */ ) {
		return imp_.is_this_mine( pick );
	}

	void set_alpha(int alpha) {
		imp_.set_alpha( alpha );
	}

	void hide_outline() {
		imp_.hide_outline();
	}

	void show_outline() {
		imp_.show_outline();
	}

	void explosion_setup()        { imp_.explosion_setup(); }
	void explosion_loop(int step) { imp_.explosion_loop(step); }
	void explosion_cleanup()      { imp_.explosion_cleanup(); }

private:
    Location loc_;
    int size_;
    Color const* color_;

    Imp imp_;
};

#endif
