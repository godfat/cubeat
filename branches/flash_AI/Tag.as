
//almost the whole Tag has been rewritten

class Tag{
    public function Tag(id: Number) {
		id_ = id;
		num_of_tag_ = 0; 
		chainlist_ = new Array(0);
	}

	public function rear(): Number{ return chainlist_[ chainlist_.length-1 ]; }
	public function add_combo(num: Number): Void {
		chainlist_[ chainlist_.length-1 ] += num;
		//always add onto the rear element
		//map will be responsible to notify tag to push new elements to tags.
		trace( chainlist_ );
	}
	public function add_num_of_combo(): Void {
		chainlist_.push( 0 );
	}
	public function inc_tag(): Void { ++num_of_tag_; /*trace("ADD:num of tag " + id_ + ": " + num_of_tag_);*/ }
	public function dec_tag(): Void { --num_of_tag_; /*trace("SUB:num of tag " + id_ + ": " + num_of_tag_);*/ }
	public function num_of_alive_tag(): Number { return num_of_tag_; }
	public function id(): Number { return id_; }

	public function attack_factor(): Number { 
		var num: Number = 0;
		for(var i=0; i < chainlist_.length; ++i ) {
//			(方塊消除數目-3)*連鎖段數+2^(連鎖段數-2) 
			if( chainlist_[i] < 3 ) break;
			if( i == 0 )
				num += (chainlist_[i] - 3)*(i+1);
			else
				num += (chainlist_[i] - 3)*(i+1) + Math.pow(2, i-1);
		}
		return num;
	}
	
	public function score_count(): Number {   //this returns the score fragment made by the last combo
		return chainlist_[ chainlist_.length-1 ] * 20 * chainlist_.length;
	}

    private var id_: Number;
	private var chainlist_: Array;
	private var num_of_tag_: Number;
}
