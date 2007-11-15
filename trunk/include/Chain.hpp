
/*2007.11.9 
  Most of the things remain unchanged, only did some clean up here.
  
  These included file is going to be heavily modified:
  1. Sound.hpp
*/          

#ifndef _SHOOTING_CUBE_Chain_HPP_
#define _SHOOTING_CUBE_Chain_HPP_

#include "PooledObject.hpp"
#include <vector>

#include "Sound.hpp"

namespace{

inline int power2(int times){
    int result = 1;
    for(int i=0; i<times; ++i) result *= 2;
    return result;
}

} // local namespace

class Chain: public PooledObject
{
public:
    int last_step_amounts() const{ return chain_step_.back(); }

    void add_last_step_amounts(int num){
		chain_step_.back() += num;
		//always add onto the rear element
		//map will be responsible to notify Chain to push new elements to Chains.
	}

	void step_one() { 
		chain_step_.push_back(0); 
		Sound::i().combo( chain_step_.size() );
	}

    void inc_one_owner(){ ++owner_amounts_; }
	void dec_one_owner(){ --owner_amounts_; }
	int owner_amounts() const{ return owner_amounts_; }

    int power_factor() const{
		int result = 0;
		for(int i=0, iend=chain_step_.size(); i!=iend; ++i){
			if( chain_step_[i] < 3 ) break;
			if( i == 0 )
				result += (chain_step_[i] - 2)*(i+1);
			else
				result += (chain_step_[i] - 2)*(i+1) + power2(i-1);
		}
		return result;
	}

    //this returns the score fragment made by the last combo
	int score_step() const{ 
		return chain_step_.back() * 20 * chain_step_.size();
	}

private:
    virtual void do_init(){
        owner_amounts_ = 0;
        chain_step_.clear();
    }
    virtual void do_suicide(){}

private:
    int owner_amounts_;
    std::vector<int> chain_step_;
};

#endif
