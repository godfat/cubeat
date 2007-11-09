
/* 2007.11.9
   Nothing changed.

  These included file are going to be modified:
  1. Map.hpp
  2. Square.hpp
*/

#ifndef _SHOOTING_CUBE_ONE_FADING_HPP_
#define _SHOOTING_CUBE_ONE_FADING_HPP_

#include "Chain.hpp"
#include "Map.hpp"
#include "Factory.hpp"
#include "Square.hpp"

class OneFading{
    typedef Map::slist_type slist_type;
public:
    // begin only used by Map::next_state
    OneFading(Map& map, slist_type const& list):
        map_(map), list_(list)
    {}
    slist_type::size_type size() const{ return list_.size(); }
    void fade(Square* that_would_be_killed_by_map){

        Chain* chain = get_the_chain();

        for(slist_type::iterator i=list_.begin(), iend=list_.end();
            i!=iend; ++i)
        {
            map_.squares_belong_to_the_chain_from_bottom_to_top(*i, chain);
            if( *i != that_would_be_killed_by_map ){
                (**i).go_dying();
                (**i).cycled();
            }
        }
		// map_.squares_belong_to_the_chain_from_bottom_to_top(*--i, chain);
    }
    // end

private:
    Chain* get_the_chain(){
        Chain* chain = NULL;

        //this for loop will get the first Chain it finds in this column or row
        for(slist_type::iterator i=list_.begin(), iend=list_.end();
            i != iend; ++i)
        {
            if( (**i).chain() ) {
                chain = (**i).chain();
                break;
            }
        }

        //if we can't find any useful Chain here, we'll create one
        if( !chain ) {
            chain = SF::Instance().Create<Chain>();//->init( map_.num_of_chains() );
            chain->step_one();
            map_.push_chain( chain );
        }

        //and add combo to it
        chain->add_last_step_amounts( list_.size() );

        return chain;
    }

private:
    Map& map_;
    slist_type list_;
};

#endif
