
import flash.geom.ColorTransform;
import flash.geom.Transform;

class Square
{
	public function clone(clone_map: Map): Square{
		var clone_square = new Square(clone_map, x_, y_, color_num, true);
		//var depth: Number = map_.next_depth();
        //clone_square.body_ = map_.body.attachMovie("Square", "Square"+clone_square.depth, clone_square.depth, {_x: x*map_.Size, _y: y*map_.Size});
        //clone_square.body_.self = clone_square;
		
		//changed a lot, but I think Controller shouldn't be made like this.....
       /* body_.onPress = function()
		{
			self.i_am_hit();
        } */

        //trans_ = new Transform(body_.inner);
        //color_ = new ColorTransform();
		
		//choose_color( color );   //separated
		//color_num = color;
        
		//trans_.colorTransform = color_;
        //map_.setup(x, y, this);
        //state_ = map_.next_state(this);
		if(this.state_ instanceof Waiting){
			clone_square.state_ = new Waiting(clone_map);
		}else if(this.state_ instanceof Dropping){
			clone_square.state_ = new Dropping(clone_map);
		}else{
			clone_square.state_ = null;
		}
		clone_square.cycled = false;
		clone_square.first_drop = this.first_drop;
		//Gray.redOffset = 128;
		//Gray.greenOffset = 128;
		//Gray.blueOffset = 128;
		clone_square.set_hp(this.hp_);
		clone_square.is_garbage_ = this.is_garbage_;
		clone_square.is_broken_ = this.is_broken_;

        //clone_square.debug_tag  = this.body_.createTextField("tag"+clone_square.depth, clone_map.next_depth(), 18, 0, 18, 18);
		//clone_square.debug_tag2 = this.body_.createTextField("_tag"+clone_square.depth, clone_map.next_depth(), 2, 10, 60, 18);
		//clone_square.debug_tag4 = this.body_.createTextField("___tag"+clone_square.depth, clone_map.next_depth(), 2, 20, 60, 18);
		//clone_square.debug_tag5 = this.body_.createTextField("____tag"+clone_square.depth, clone_map.next_depth(), 2, 30, 60, 18);
		clone_square.tag_ = this.tag_;
		
		return clone_square;
		
		//private function real_y_position(): Number{ return y_*map_.Size; }
		//public var color_num: Number;
		//private var x_: Number;
    	//private var y_: Number;
    	//private var body_: MovieClip;
    	//private var state_: State;
    	//private var map_: Map;
    	//private var trans_: Transform;
    	//private var color_: ColorTransform;
		//private var tag_: Tag = null;
		//private var debug_tag: TextField;
		//private var debug_tag2: TextField;
		//private var debug_tag3: TextField;
		//private var debug_tag4: TextField;
		//private var debug_tag5: TextField;
		//added
		//public var cycled:Boolean;
		//private var hp_: Number;
		//private var is_garbage_: Boolean;
		//public function set_hp(hp: Number): Void { hp_ = hp; }
		//public function dec_hp(dmg: Number): Void { hp_ -= dmg; }
		// dummy
		//private var self: Square;
	}
	public function Square(map: Map, x: Number, y: Number, color:Number)
	{
        map_ = map;
        x_ = x;
        y_ = y;
        if(map_.for_clone != true){
			var depth: Number = map_.next_depth();
			body_ = map_.body.attachMovie("Square", "Square"+depth, depth, {_x: x*map_.Size, _y: y*map_.Size});
			body_.self = this;
		}
		//changed a lot, but I think Controller shouldn't be made like this.....
       /* body_.onPress = function()
		{
			self.i_am_hit();
        } */

        trans_ = new Transform(body_.inner);
        color_ = new ColorTransform();
		
		choose_color( color );   //separated
		color_num = color;
		first_drop = true;
        
		trans_.colorTransform = color_;
        map_.setup(x, y, this);
        //state_ = map_.next_state(this);
		state_ = map_.make_Waiting();
		
		//added
		cycled = false;
		Gray.redOffset = 128;
		Gray.greenOffset = 128;
		Gray.blueOffset = 128;
		hp_ = 1;
		is_garbage_ = false;
		is_broken_ = false;
		if(map_.for_clone != true){
			debug_tag  = body_.createTextField("tag"+depth, map_.next_depth(), 18, 0, 18, 18);
			debug_tag2 = body_.createTextField("_tag"+depth, map_.next_depth(), 2, 10, 60, 18);
			debug_tag4 = body_.createTextField("___tag"+depth, map_.next_depth(), 2, 20, 60, 18);
			debug_tag5 = body_.createTextField("____tag"+depth, map_.next_depth(), 2, 30, 60, 18);
		}
    }
	
	//added
	public function i_am_hit(dmg: Number): Void {
		if( map_.id() == Player.active_player() ) {
			if( is_broken_ == true ) restore();
			else hp_ -= dmg;
		}
		
		else {
			if( is_garbage_ == true ) return;
			else if( is_broken_ == true ) hp_ -= dmg;
			else broken();
		}
					
		if( hp_ <= 0 ) {
			state_ = map_.make_Dying();
			if( map_.id() == Player.active_player() ) 
				map_.score += 10;
			cycled = true;
		}
	}
	
	//separated
	private function choose_color( color: Number ): Void {
		switch(color){
            case 0: color_.redOffset = 255; break;
            case 1: color_.greenOffset = 255; break;
            case 2: color_.blueOffset = 255; break;
            case 3: color_.redOffset = color_.greenOffset = 255; break;
            case 4: color_.redOffset = 128; color_.blueOffset = 255; break;
            case 5: color_.redOffset = 255; color_.greenOffset = 128; break;
            case 6: color_.blueOffset = 255; color_.greenOffset = 255; break;
            case 7: color_.greenOffset = 128; break;
        }
	}

    public function cycle_and_die(): Boolean{
				
		if( state_ instanceof Waiting && Waiting(state_).state_checked() ) {
			debug_tag5.text = "checked"
			this.first_drop = false;
		}
		else { debug_tag5.text = ""; }
		
		if( cycled ) {
			debug_tag2.text = "Cycled";
			return false;  //added: important we break the function here 
		}
		else {
			debug_tag2.text = "";
		}
		
		var next_state:State = state_.cycle(this);

		debug_tag4.text = next_state.toString();
		
		if( next_state != null ){
			if( next_state != state_ ) {
				state_ = next_state;
			}
			return false;
		}
		else{
			this.suicide();
			return true;
		}
	}

    public function suicide(): Void{
        body_.removeMovieClip();
        delete this;
    }

    public function isNotReached(): Boolean{ return body_._y < real_y_position(); }
    public function drop(): Void{ body_._y += map_.Speed; }
	public function drop_a_block(): Void{ ++y_; }
    public function addjust(): Void{ body_._y = real_y_position(); }
    public function get x(): Number{ return x_; }
    public function get y(): Number{ return y_; }
    public function set y(y: Number): Void{ y_ = y; }
    public function get body(): MovieClip{ return body_; }
    public function get rgb(): Number{ return color_.rgb; }
    public function get state(): State{ return state_; }
    public function set state(state: State): Void{ state_ = state; }
    public function set tag(new_tag: Tag): Void { 
		tag_ = new_tag; 
		debug_tag.text = tag_.id().toString(); 
	}
    public function get tag(): Tag{ return tag_; }
	
	//added something
    public function clear_tag(): Void { 
		tag_.dec_tag();
		tag_ = null; debug_tag.text = ""; 
	}

    private static var Black: ColorTransform = new ColorTransform;
	private static var Gray:  ColorTransform = new ColorTransform;    //added
	
    public function be_garbage(): Void{
		is_garbage_ = true;
		hp_ = 3;
        color_.rgb = Math.floor(Math.random()*16777216);
        trans_.colorTransform = Black;
    }
	
	//added
	private var is_broken_: Boolean;
	public function broken(): Void{
		color_.rgb = Math.floor(Math.random()*16777216);
		trans_.colorTransform = Gray;
		is_broken_ = true;
	}
	
	//added
	public function restore(): Void{
		color_.rgb = 0;      //crappy ..........
		choose_color( Math.floor( Math.random()*map_.Color_Amounts ) );
		trans_.colorTransform = color_;
		is_broken_ = false;
		if( state_ instanceof Waiting && !Waiting(state_).needCheck() )
			Waiting(state_).uncheck();
	}

    private function real_y_position(): Number{ return y_*map_.Size; }
	public var color_num: Number;
	public var first_drop: Boolean;
	private var x_: Number;
    private var y_: Number;
    private var body_: MovieClip;
    private var state_: State;
    private var map_: Map;
    private var trans_: Transform;
    private var color_: ColorTransform;
	
    private var tag_: Tag = null;

	private var debug_tag: TextField;
	private var debug_tag2: TextField;
	private var debug_tag3: TextField;
	private var debug_tag4: TextField;
	private var debug_tag5: TextField;
	
	//added
	public var cycled:Boolean;
	private var hp_: Number;
	private var is_garbage_: Boolean;
	public function set_hp(hp: Number): Void { hp_ = hp; }
	public function dec_hp(dmg: Number): Void { hp_ -= dmg; }
    // dummy
    private var self: Square;
}
