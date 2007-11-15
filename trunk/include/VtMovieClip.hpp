
/*2007.11.9 
  cleaned up things which were coupled with Virtools SDK,
  this file is deprecated, reserved only for reference uses,
  useless code blocks are removed.

  This file won't compile unless further changes made.

  Filename and classes contained are going to be renamed.
*/         

#ifndef _SHOOTING_CUBE_VT_MOVIE_CLIP_HPP_
#define _SHOOTING_CUBE_VT_MOVIE_CLIP_HPP_

#include "MovieClip.hpp"
#include "Location.hpp"
#include "Color.hpp"

#include "Debugger.hpp"

class VtMovieClip
{
public:
    VtMovieClip(){
        //might-be-useful snippets
		//explosion_mesh = NULL;
		//original_mesh = body_->GetCurrentMesh();
	}

    ~VtMovieClip(){
    }

    //template <class Callback>
    void init(Location const& loc, Color const& color, int size, /* Something* parent_ent */ ){
        /* blah blah blah */
    }

    void cycle(Location const& new_loc, Color const& new_color){
        /* blah blah blah */
    }

    void hide(){
        /* blah blah blah */
    }

	//added
	void be_garbage()
	{   /*
		VxColor vcolor(40,40,40,192);
	    VxColor vcolor2(128,128,128);
        body_->GetMesh(0)->GetMaterial(0)->SetDiffuse(vcolor);
		body_->GetMesh(0)->GetMaterial(0)->SetEmissive(vcolor/=3);
		body_->GetChild(0)->GetMesh(0)->GetMaterial(0)->SetDiffuse(vcolor2);
		body_->GetChild(0)->GetMesh(0)->GetMaterial(0)->SetEmissive(vcolor2/=2); */
	}

	void be_broken()
	{   /*
		VxColor vcolor(133,133,133,192);
		VxColor vcolor2(150,150,150);
		body_->GetMesh(0)->GetMaterial(0)->SetDiffuse(vcolor);
		body_->GetMesh(0)->GetMaterial(0)->SetEmissive(vcolor/=3);
		body_->GetChild(0)->GetMesh(0)->GetMaterial(0)->SetDiffuse(vcolor2);
		body_->GetChild(0)->GetMesh(0)->GetMaterial(0)->SetEmissive(vcolor2); */
	}

	void update_color( Color const& color )
	{   /*
		color_ = color;
		VxColor vcolor(color.r(), color.g(), color.b(), 192 );
		VxColor vcolor2(color.r(), color.g(), color.b());
        body_->GetMesh(0)->GetMaterial(0)->SetDiffuse(vcolor);
		body_->GetMesh(0)->GetMaterial(0)->SetEmissive(vcolor/=3);
		body_->GetChild(0)->GetMesh(0)->GetMaterial(0)->SetDiffuse(vcolor2);
		body_->GetChild(0)->GetMesh(0)->GetMaterial(0)->SetEmissive(vcolor2);  */
	}

	void set_alpha( int alpha ) {  /*
        VxColor vcolor(255, 255, 255, alpha );
		body_->GetMesh(0)->GetMaterial(0)->SetDiffuse(vcolor);
		body_->GetMesh(0)->GetMaterial(0)->SetEmissive(vcolor/=3);  */
	}

	void hide_outline() {
		/* blah */
	}

	void show_outline() {
		/* blah */
	}

	//added: I AM SOOOOOOOOO UGLY!!!!!
	bool is_this_mine( /* Something* pick */ )
	{
		return false; /* blah */
	}

	void explosion_setup() 
	{
        /* that explosion method used in Virtools SDK is not 
           suitable for further usage, so removed. */
	}

	void explosion_loop(int step)
	{
		/* that explosion method used in Virtools SDK is not 
           suitable for further usage, so removed. */
	}

	void explosion_cleanup()
	{
		/* that explosion method used in Virtools SDK is not 
           suitable for further usage, so removed. */
	}

private:
    Location loc_;
    Color color_;

    /* Something* body_; */
	/* Something* parent_ent_; */
    
    VtMovieClip(VtMovieClip const&);

	//added for explosion
	/* Something* explosion_mesh; */
	/* Something* original_mesh; */
	/* SomeVector center; */

	int elapsedTime;
	int time;
	float value, lastValue;
	float maxdistance;
};

#endif
