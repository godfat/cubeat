
/* 2007.11.9
   You see, this thing is just a total bullshit, 
   and actually most of the things WILL NOT be the same anymore
  (even for structural issues), so I'll just keep a few functions in this
   file for references.

   Basically this file is retarded.
*/

#include "Game.hpp"
#include "Input.hpp"
#include "MC.hpp"
#include "boost/bind.hpp"
#include <sstream>
#include "Debugger.hpp"

void single_init(){
	Game::PUZZLE_RETRY = 0;
	Sound::i().play_music( SG::Instance().random(3)+1 );
	SM::Instance().disable();
	SS::Instance().enter();
    SG::Instance().single_init();
    SM::Instance().hide();
}

void multi_init(){
	Game::PUZZLE_RETRY = 0;
	Sound::i().play_music( SG::Instance().random(3)+1 );
	SM::Instance().disable();
	SS::Instance().enter();
    SG::Instance().multi_init();
    SM::Instance().hide();
}

void puzzle_init(){
	Game::PUZZLE_RETRY = 3;
	Sound::i().play_music( SG::Instance().random(3)+1 );
	SM::Instance().disable();
	SS::Instance().enter();
    SG::Instance().puzzle_init();
    SM::Instance().hide();
}

int SC_Core(CKBehaviorContext const& behcontext) {

	//Game.hpp will be infected by CK and Vx things for now
	//We should buildup wrappers afterwards

	Input::i().set_all_value( /* blah blah blah */ );

	SS::Instance().cycle();

    if( SM::Instance().is_shown() ){

		Sound::i().play_music(0);

		if( SM::Instance().is_enable() ) {
			static bool d_clicked_ = false;
			if( Input::i().D1() ){
				if( d_clicked_ )
					d_clicked_ = false;
				else
					d_clicked_ = true;
			}
			else{
				d_clicked_ = false;
			}

			VxIntersectionDesc res1;
			SM::Instance().cycle(
				static_cast<CK3dEntity*>(context->GetPlayerRenderContext()->Pick(Input::i().pos1().x, Input::i().pos1().y, &res1)),
				d_clicked_);
		}

	    int result = 0;
        beh->SetOutputParameterValue(0, &result);
    }

    else{

	    int result = SG::Instance().cycle();
        beh->SetOutputParameterValue(0, &result);
        if(result != 0){
            Loki::DeletableSingleton<Game>::GracefulDelete();
            if(result > 0){
                SM::Instance().show();
				SS::Instance().leave();
            }
            else{ // in stage mode...
                if(result == -11 || result == -4444){
                    SM::Instance().show();
					SS::Instance().leave();
                }
                else{
                    SG::Instance().puzzle_init(-result);
                }
            }
        }
    }

	return CKBR_OK;
}

