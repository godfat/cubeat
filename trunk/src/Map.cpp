
#include "Map.hpp"
#include "Game.hpp" // for random
#include "Factory.hpp" // for Create
#include "Square.hpp" // for many
#include "OneFading.hpp" // this is only used here for Map

#include "Sound.hpp"

// added

Map::~Map(){
    for(container_type::reverse_iterator i=data_.rbegin(), iend=data_.rend();
        i!=iend; ++i)
    {
        if(Square* s = *i){
            s->suicide();
        }
    }

	for( unsigned int i=0; i < preview_.size(); ++i ) 
		if( preview_[i] )
			preview_[i]->suicide();
	preview_.clear();

	//delete the map ent_ here
	context->DestroyObject(ent_, CK_DESTROY_TEMPOBJECT);
	for( unsigned int i = 0; i < warning_strip_.size(); ++i ) {
        CKDependencies d;
        d.m_Flags = CK_DEPENDENCIES_FULL;
        context->DestroyObject( warning_strip_[i], CK_DESTROY_TEMPOBJECT, &d);
	}

	Debugger::i().trace("Map Destroyed!");
}

// only used by Game
void Map::insert(Square* that, int x, int y){
    that->init(this, x, y);
    data_.at(x, y) = that;
}

void Map::insert(Square* that, int x, int y, int color_id){
    that->init(this, x, y, color_id);
    data_.at(x, y) = that;
}

void Map::insert_garbage(Square* that, int x, int y){
    that->init(this, x, y);
	that->be_garbage();
    data_.at(x, y) = that;
}

void Map::fill_preview_queue(){

	if( preview_.size() != 0 ) return;

	std::vector<int> preview_set( ms().width );           //color set
	int count = 0;
	if( SG::Instance().random(2) ) { 
		preview_set[0] = preview_set[1] = SG::Instance().random( ms().color_amounts );
		count = 1;
	} else preview_set[0] = SG::Instance().random( ms().color_amounts );

	for( ; count < ms().width-1; ) {
		if( count == ms().width-2 ) fill1( preview_set, count );
		else {
			if( SG::Instance().random(2) ) fill2( preview_set, count );
			else                           fill1( preview_set, count );
		}
	}

	for( int xx=0; xx < ms().width; ++xx ) 
	    preview_.push_back( SF::Instance().Create<Square>()->init(this, xx, -1, preview_set[xx]) );
}

void Map::fill2(std::vector<int>& preview_set, int& count) {
    count += 2;  //add length 2 here
    int temp;
	do {
		temp = SG::Instance().random( ms().color_amounts );
	} while( temp == preview_set[ count-2 ] );
	preview_set[ count-1 ] = preview_set[ count ] = temp;
}

void Map::fill1(std::vector<int>& preview_set, int& count) {
	++count;     //add length 1 here
	int temp;
	do { 
		temp = SG::Instance().random( ms().color_amounts ); 
	} while( temp == preview_set[ count-1 ] );
    preview_set[ count ] = temp;
}

// only used by Game
void Map::cycle(){
	if( SG::Instance().game_mode() != 2 ) {
		int speed_up_limit = (SG::Instance().game_mode() == 0 ? 13 : 7); 
		int speed_limit = (SG::Instance().game_mode() == 0 ? 4 : 8);
		if( (time(0) - speed_up_step_) >= speed_up_limit && map_setting_.speed < speed_limit ){
			map_setting_.speed *= 1.1;
			speed_up_step_ = time(0);
			if( map_setting_.damage_factor < 2.0 ) 
				map_setting_.damage_factor *= 1.05;
		}
	}

	/* modified: freeze frequency setting now
	if( (time(0) - freq_up_step_) >= 16 && map_setting_.frequency > 100 ){
		map_setting_.frequency -= 5;
		freq_up_step_ = time(0);
	}*/

    refresh_squares(); // make square to cycle

	update_chains();
    cycle_squares();
	cycle_creation_garbage(); //added
	if(ms().dropping_creatable) {
	    cycle_creation();  //added
		cycle_warning(); //added
	}
}

void Map::refresh_squares(){
    for(container_type::reverse_iterator i=data_.rbegin(), iend=data_.rend();
        i!=iend; ++i)
    {
        if(Square* s = *i){
            s->refresh();
        }
    }
}

void Map::cycle_squares(){

	n_of_newcomers_ = 0;

    for(container_type::reverse_iterator i=data_.rbegin(), iend=data_.rend();
        i!=iend; ++i)
    {
        if(Square* s = *i){
			if( !s->has_grounded() ) ++n_of_newcomers_;
            if( s->cycle_and_die() ){
                *i = NULL;
            }
        }
    }
}

bool Map::check_column_full()
{
	int counter = 0;
	for( int xx=0; xx < ms().width; ++xx ) {
		if( data_.at(xx, 1) && data_.at(xx, 1)->is_waiting() ) {
			counter++;
			warning_strip_[xx]->Show(CKSHOW);
		}
		else warning_strip_[xx]->Show(CKHIDE);
	}

	if( counter > 0 )
		return true;
	else 
		return false;
}

//added: split warning check from cycle_creation:
void Map::cycle_warning()
{
    if( ++creation_step_ > ms().frequency ) {
		if( check_column_full() ){ // warning!!!
			if( lose_warning_step_ == 0 ){
				counting_num = 4;
                lose_warning_step_ = time(0);
            }
			if( counting_num >= 0 ) {
				SG::Instance().yell_warning(this, counting_num );
				--counting_num;
				Sound::i().countdown();
			}
			else SG::Instance().game_over(this);
        }
        else{
            SG::Instance().no_warning(this);
            lose_warning_step_ = 0;
        }
		creation_step_ = 0;
	}
}

//added
void Map::cycle_creation()
{
    if( garbage_left_ > 0 || n_of_newcomers_ > 0 || garbage_switch_ == true )
		return;

	// we must check which column's top is empty when drop squares
	// so we setup an array of columns which its top is empty
	std::vector<int> is_empty;
	for( int xx=0; xx < ms().width; ++xx )
		if( !data_.at(xx, 0) && !data_.at(xx, 1) )
			is_empty.push_back( xx );

	if( is_empty.size() == 0 ) return;
	//and find one column in this array to drop randomly
	//int x = is_empty[ SG::Instance().random( is_empty.size() ) ];

	for( unsigned int i=0; i < is_empty.size(); ++i ) {
		preview_[ is_empty[i] ]->init2( this, is_empty[i], 0 );
		data_.at( is_empty[i], 0 ) = preview_[ is_empty[i] ];
        preview_[ is_empty[i] ] = 0;
	}

	for( unsigned int i=0; i < preview_.size(); ++i ) {
        if( preview_[i] )
        preview_[i]->suicide();
	}
	preview_.clear();

	fill_preview_queue();

	if( SG::Instance().game_mode() == 1 ) //if single
		garbage_left_ += SG::Instance().random(3);
}

//added: here has somebug? fixed
void Map::cycle_creation_garbage()
{
    // no garbage no working
	if( garbage_left_ < 1 || ( n_of_newcomers_ > 0 && garbage_switch_ == false ) ) {
		garbage_switch_ = false;
        return;
	}

	garbage_switch_ = true;

    std::vector<int> is_empty;

	//check empty columns
	for( int xx=0; xx < ms().width; ++xx )
		if( !data_.at(xx, 0) && !data_.at(xx, 1) )
			is_empty.push_back( xx );

	//shuffle the array
	for( unsigned int i=0; i < is_empty.size(); ++i ) {
		int rand = SG::Instance().random( is_empty.size() );
		std::swap( is_empty[i], is_empty[ rand ] );
	}

	//drop them according to the empty column
	for( unsigned int i=0; i < is_empty.size() && garbage_left_ > 0 ; ++i ) {
		insert_garbage(SF::Instance().Create<Square>(), is_empty[i], 0 );
		--garbage_left_;
	}
}

void Map::update_chains(){
    for(std::vector<Chain*>::iterator i=chains_.begin();
        i != chains_.end(); ++i)
    {
		if( (**i).last_step_amounts() != 0 ) {          //when the Chain's rear is not zero we notify to add a new combo
			score_ += (**i).score_step() * ms().damage_factor;  //damage_factor to score adjustment (1.00 ~ 2.00)
			(**i).step_one();
		}

		if( (**i).owner_amounts() == 0 ) {   //when the Chain's all dead then we fire up the attack and make this one null
			SG::Instance().make_garbage( this, (**i).power_factor() * ms().damage_factor );  //damage_factor to score adjustment (1.00 ~ 2.00)
            (**i).suicide();
            i = chains_.erase(i);
            --i;
		}
	}
}

// only used by Square::init
Color Map::random_color() const
{
	return id_color(SG::Instance().random(ms().color_amounts));
}

Color Map::id_color(int color_id) const{
    enum{r,g,b};
    int rgb[3]={0};
    switch( color_id ){
        case 0: rgb[r] = 255; break;
        case 1: rgb[g] = 255; break;
        case 2: rgb[b] = 255; break;
        case 3: rgb[r] = rgb[g] = 255; break;
        case 4: rgb[r] = 128; rgb[b] = 255; break;
        case 5: rgb[r] = 255; rgb[g] = 128; break;
        case 6: rgb[b] = rgb[g] = 255; break;
        case 7: rgb[g] = 128; break;
        default: assert(0); // too many...
    }
	return Color(rgb[r], rgb[g], rgb[b]);
}

Color Map::garbage_color() const{
	return Color(SG::Instance().random());
}

// used by Dropping::cycle, Waiting::cycle
State* Map::next_state(Square* that){
    if( Waiting* waiting = that->is_waiting() ){

		if( waiting->need_check() ){
            OneFading* one_fading = make_OneFading(that);

            if( one_fading != NULL ){
                one_fading->fade(that); delete one_fading;
                return SF::Instance().Create<Dying>()->init(that)->init2();
            }
            waiting->checked();
			that->set_grounded();         //once grounded, always grounded
        }
		if( waiting->need_check_chain_property() ){
			if( square_can_lose_chain(that) ) { 
	            that->lose_chain();
				if( !that->is_garbage() )
				    that->hide_outline();
			}
			waiting->chain_property_checked();
		}
    }

    if( is_below_empty(that) ){
        data_.at(that->x(), that->y()) = NULL;
        that->drop_a_block();
        data_.at(that->x(), that->y()) = that;
        if( that->is_dropping() )
            return NULL;
        else
            return SF::Instance().Create<Dropping>()->init(that);
    }
    else{
        if( that->is_waiting() )
            return NULL;
		else {
			Sound::i().cube_dropped();
            return SF::Instance().Create<Waiting>()->init(that);
		}
    }
}

bool Map::square_can_lose_chain(Square const* that) const{
	for( int x = that->x(), y = that->y()+1; y<ms().height; ++y )
	    if( data_.at(x, y) == NULL || data_.at(x, y)->is_dying() || data_.at(x, y)->is_exploding() ) 
	        return false;  //when it's empty, dying or exploding, the square ontop can't lose chain

	return true;
}

// only used by OneFading::fade
void Map::squares_belong_to_the_chain_from_bottom_to_top(Square* that, Chain* chain){
    for(int x=that->x(), y=that->y();
		data_.at(x, y) != NULL &&
		data_.at(x, y)->is_waiting() &&
        data_.at(x, y)->chain() == chain    
		; --y)
    {
        data_.at(x, y)->lose_chain_delay() = 0;
    }

	for(int x=that->x(), y=that->y();
        data_.at(x, y) != NULL &&
        data_.at(x, y)->is_waiting() &&    //added: important
		data_.at(x, y)->chain() == NULL    //added: important
		; --y)
    {
        data_.at(x, y)->chain() = chain;
		data_.at(x, y)->show_outline();
        data_.at(x, y)->lose_chain_delay() = 0;
        chain->inc_one_owner();
    }
}


//THIS IS A UGLY THING
bool Map::owner_hit_you( CK3dEntity* pick, int dmg )
{
	Debugger::i().trace(" phase1: Map.someone_got_hit ");
    for(container_type::reverse_iterator i=data_.rbegin(), iend=data_.rend();
        i!=iend; ++i)
    {
        if(Square* s = *i){
			//Debugger::i().trace(" phase2: Map.iterate through candidates ");
            if( s->is_this_mine( pick ) ) {
				Debugger::i().trace(" phase3: notify someone go dying ");
                s->owner_hit_me( dmg );
				return true;
            }
        }
    }
	return false;
}

//THIS IS A UGLY THING
void Map::enemy_hit_you( CK3dEntity* pick, int dmg )
{
	Debugger::i().trace(" phase1: Map.someone_got_hit ");
    for(container_type::reverse_iterator i=data_.rbegin(), iend=data_.rend();
        i!=iend; ++i)
    {
        if(Square* s = *i){
			//Debugger::i().trace(" phase2: Map.iterate through candidates ");
            if( s->is_this_mine( pick ) ) {
				Debugger::i().trace(" phase3: notify someone go dying ");
                s->enemy_hit_me( dmg );
				break;
            }
        }
    }
}


// private below
OneFading* Map::make_OneFading(Square* that){
    slist_type traveled, not_traveled, result;
    not_traveled.insert(that);
    make_slist(traveled, not_traveled, result);
    // Debugger::i().trace(result.size());
    return result.size() ? new OneFading(*this, result) : NULL;
}

bool Map::is_below_empty(Square* that) const{
    if( that->y()+1 >= ms().height ) return false;
    else return data_.at(that->x(), that->y()+1) == NULL;
}

void Map::make_slist(slist_type& traveled, slist_type& not_traveled, slist_type& result){
    if(not_traveled.size()){
        slist_type::iterator node = not_traveled.begin();
        Square* s = *node;

        if( traveled.count(s) == 0 ){
            make_row(s, not_traveled, result);
            make_column(s, not_traveled, result);
            traveled.insert(s);
        }

        not_traveled.erase(node);

        make_slist(traveled, not_traveled, result);
    }
}

namespace{

bool check_not_continue(std::vector<Square*>& list, Square* that, Square* ya_that){
    if( ya_that == NULL ){
        return true;
    }
    else if( *ya_that != *that ){
        return true;
    }
    else if( ya_that->is_waiting() ){
        list.push_back(ya_that);
        return false;
    }
    else{
        return true;
    }
}

void process_chaining(
    std::vector<Square*> const& row_or_column,
    Map::slist_type& not_traveled,
    Map::slist_type& result)
{
        for(std::vector<Square*>::const_iterator i=row_or_column.begin(), iend=row_or_column.end();
            i!=iend; ++i)
        {
            not_traveled.insert(*i);
            result.insert(*i);
        }
}

} // local namespace

void Map::make_row(Square* that, slist_type& not_traveled, slist_type& result){
    std::vector<Square*> row;

    for(int i=that->x(); i>=0; --i) // include self
        if( check_not_continue( row, that, data_.at(i, that->y()) ) )
            break;

    for(int i=that->x()+1; i<ms().width; ++i) // exclude self
        if( check_not_continue( row, that, data_.at(i, that->y()) ) )
            break;

    // Debugger::i().trace(row.size());

    if(static_cast<int>(row.size()) >= this->ms().chain_amounts)
        process_chaining(row, not_traveled, result);
}

void Map::make_column(Square* that, slist_type& not_traveled, slist_type& result){
    std::vector<Square*> column;

    for(int i=that->y(); i>=0; --i) // include self
        if( check_not_continue( column, that, data_.at(that->x(), i) ) )
            break;

    for(int i=that->y()+1; i<ms().height; ++i) // exclude self
        if( check_not_continue( column, that, data_.at(that->x(), i) ) )
            break;

    // Debugger::i().trace(column.size());

    if(static_cast<int>(column.size()) >= this->ms().chain_amounts)
        process_chaining(column, not_traveled, result);
}


bool Map::all_empty()
{
	int counter = 0;
    for(container_type::reverse_iterator i=data_.rbegin(), iend=data_.rend();
        i!=iend; ++i)
    {
		if(Square* s = *i) {
		    return false;
		}
    }

	return true;
}

bool Map::have_alive_chain()
{
	int counter = 0;

	if( chains_.size() > 0 ) {
		if( chains_[ chains_.size()-1 ]->owner_amounts() > 0 )
			return true;
	}
	return false;
}

bool Map::no_one_dropping()
{
	int counter = 0;
	for(container_type::reverse_iterator i=data_.rbegin(), iend=data_.rend();
        i!=iend; ++i)
    {
        if(Square* s = *i){
			if( s->is_dropping() ) {
				counter = 1;
				break;
			}
        }
    }

	return counter == 0;
}

bool Map::every_one_is_checked()
{
	int counter = 0;
	for(container_type::reverse_iterator i=data_.rbegin(), iend=data_.rend();
        i!=iend; ++i)
    {
        if(Square* s = *i){
			if( Waiting* stat = s->is_waiting() ) {
				if( stat->need_check() ) 
					return false;	
			}
			else return false;
        }
    }
	return true;
}
