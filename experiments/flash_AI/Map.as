
class Map{
    public var Width: Number;
    public var Height: Number;
    public var Size: Number;
    public var Speed: Number;
    public var Frequency: Number;
    public var Decay: Number;
    public var Delay: Number;

    public var Color_Amounts: Number;
    public var Chain_Amounts: Number;

    public var Damage_Factor: Number;
	
	//make a Map Clone
	public function clone():Map {
		var cloneMapSetting: MapSetting = new MapSetting();
		cloneMapSetting.Width = this.Width;
		cloneMapSetting.Height = this.Height;
		cloneMapSetting.Size = this.Size;
		cloneMapSetting.Speed = this.Speed;
		cloneMapSetting.Frequency = this.Frequency;
		cloneMapSetting.Decay = this.Decay;
		cloneMapSetting.Delay = this.Delay;
		
		cloneMapSetting.Color_Amounts = this.Color_Amounts;
		cloneMapSetting.Chain_Amounts = this.Chain_Amounts;
		
		cloneMapSetting.Damage_Factor = this.Damage_Factor;
		
		var cloneMap: Map = new Map(game_, cloneMapSetting, this.Size*(this.Width+9), 0, true);
		//var clonedata_ = new Array(set_.Height);
		//for(var i=0; i<set_.Height; ++i) clonedata_[i] = new Array(set_.Width);
		cloneMap.for_clone = true;
		for(var y = 0; y < set_.Height; ++y)
            for(var x = 0; x < set_.Width; ++x) {
				if( data_[y][x] != null )
                    cloneMap.data_[y][x] = this.data_[y][x].clone(cloneMap);
				else cloneMap.data_[y][x] = null;
			}

		return cloneMap;		
	}

    public function Map(game: Game, setting: MapSetting, x: Number, y: Number, to_clone: Boolean){
		
		game_ = game;
        init_setting(setting);
        var depth: Number = next_depth();
		if(to_clone != true){
			for_clone = false;
			body_ = game_.body.createEmptyMovieClip("map"+depth, depth);
			body_._y = -set_.Size;
			body_._x += x;
			body_._y += y;
			depth = next_depth();
			info_ = game_.body.createTextField("info"+depth, depth, 0, 0, 80, 70);
			update_info_board();
		}else{
			for_clone = true;
		}
        data_ = new Array(set_.Height);
        for(var i=0; i<set_.Height; ++i) data_[i] = new Array(set_.Width);

        for(var y=0; y<set_.Height; ++y)
            for(var x=0; x<set_.Width; ++x)
                data_[y][x] = null;

        /*depth = next_depth();
        info_ = game_.body.createTextField("info"+depth, depth, 0, 0, 80, 70);
        update_info_board();*/
		
		//added
		taglist_ = new Array(0);
		++ID;
		id_ = ID;
		garbage_buffer_ = 0;
		garbage_step_ = 0;
		
    }

    public function move_info_board(x: Number, y: Number): Void{ info_._x = x; info_._y = y; }
	
	//changed
    public function update_info_board(): Void { 
		info_.text = "S: "+score_.toString();
		info_.text +="\nW: "+player_.weapon().toString();
		info_.text +="\nA: "+player_.weapon().ammo().toString();
		info_.text +="\nD: "+player_.weapon().delaytime().toString();
	}
	
    public function set score(new_score: Number): Void{ score_ = new_score; update_info_board(); }
    public function get score(): Number{ return score_; }
    // cool, it should be public or there's strange compile error
    // stupid as2
	// original chain related thing deleted

    public function cycle(): Void{
		
		update_info_board();  //add
		//added: clear cycled tag
		for(var y=set_.Height-1; y>=0; --y)
            for(var x=set_.Width-1; x>=0; --x)
				lookup(x, y).cycled = false;
				
		tag_update();
        cycle_squares();
		garbage_drop();    // because garbage drop will interrupt "step_", so we put garbage_drop before cycle_dropping
        cycle_dropping();
    }
	
	//added
	private function tag_update(): Void {
		for(var i=0; i < taglist_.length; ++i)  {
			if( taglist_[i] == null ) continue;           //when the tag is null we skip
			else 
			{
				if( taglist_[i].rear() != 0 ) {          //when the tag's rear is not zero we notify to add a new combo
					score += taglist_[i].score_count();
					trace( "tag " + (i+1) + " attack factor: " + taglist_[i].attack_factor() ) ;
					taglist_[i].add_num_of_combo();
				}
				
				if( taglist_[i].num_of_alive_tag() == 0 ) {   //when the tag's all dead then we fire up the attack and make this one null
					game_.make_garbage( this, taglist_[i].attack_factor() );
					taglist_[i] = null; //can i do this ?
				}
			}
		}
	}

    private function cycle_squares(): Void {
        for(var y=set_.Height-1; y>=0; --y) {
            for(var x=set_.Width-1; x>=0; --x) {
					
                if( lookup(x, y) != null &&
                    lookup(x, y).cycle_and_die() )
                {
                    // delete data_[y][x];
                    setup(x, y, null);
                }
            }
        }
    }

	//changed a lot
    private function cycle_dropping(): Void{
        if( ++step_ > set_.Frequency ){
			var cube_pool: Array = falling_dropping();
			trace(cube_pool);
			//test get Data
			/*var check_data: Array = this.getData();
			for(var gy = 0; gy < set_.Height; ++gy){
				trace(check_data[gy]);
			}
			check_data[10][0].suicide();*/
			// we must check which column's top is empty when drop squares
			// so we setup an array of columns which its top is empty
			var is_empty: Array = new Array(0);         
			for( var xx = 0; xx < set_.Width; ++xx )       
				if( lookup(xx, 0) == null && lookup(xx, 1) == null )
					is_empty.push( xx );                
			
			//and drop all column in this array 
			for(var i = 0; i < is_empty.length; ++i){
				new Square(this, is_empty[i], 0, cube_pool[i]);
			}
                       
            step_ = 0;
        }
    }
	
	//make cube_pool to drop
	private function falling_dropping(): Array{
		
		var color_pool: Array =[0,0,1,1,2,2,3,3];
		for(var i=0; i < color_pool.length; ++i ){
			var temp_Color: Number = color_pool[i];
			var temp_Local: Number = Math.floor(Math.random()*7);
			color_pool[i]=color_pool[temp_Local];
			color_pool[temp_Local] = temp_Color;
		}
		return color_pool.slice(0,6);
	}
	
	//added
	private function garbage_drop(): Void {
		if( ++garbage_step_ > set_.Frequency ) {
			if( garbage_buffer_ == 0 ) return;
			
			step_ = 0;  //when there's garbages going to drop, we notify this so we break the normal dropping cycle
			
			for( var x=0; x < set_.Width && garbage_buffer_ > 0; ++x ) {
				if( lookup(x, 0) == null ) {
					new Square(this, x, 0).be_garbage();
					--garbage_buffer_;
				}
			}
			garbage_step_ = 0;
		}
	}
	
	//changed
    public function make_garbage(amounts: Number): Void{ garbage_buffer_ += amounts; }

    public function next_state(that: Square): State{
        if( that.state instanceof Waiting ){
            var waiting: Waiting = Waiting(that.state);
            if( waiting.needCheck() ){				
				var column: OneFading = make_OneFading(that, make_column(that));
                var row: OneFading = make_OneFading(that, make_row(that));

                if( row != null || column != null ) {
                // is there any better way to do this?
                    if( row != null ) row.fade();
                    if( column != null ) column.fade();
                    //garbage attack removed to elsewhere
                    return game_.make_Dying(this); // not a big problem here 
                } 
                waiting.checked(); 
				if( tagCanClear(that) )            // no problem lah muahahahahahaha
					that.clear_tag();                   
            }
        }

        if( isBelowEmpty(that) ){
			setup(that.x, that.y, null);
			that.drop_a_block();
			setup(that.x, that.y, that);
			if( that.state instanceof Dropping ) return null;
            else return game_.make_Dropping(this, that);
        }
        else{
            if( that.state instanceof Waiting )
                return null;
            else
                return game_.make_Waiting(this);
        }
    }

    private function make_OneFading(square: Square, array: Array): OneFading{
		array.push(square);
        return array.length >= Chain_Amounts ? new OneFading(this, array) : null;
    }

    public function make_row(that: Square): Array{
        var result: Array = new Array();

        for(var i=that.x-1; i>=0; --i)
            if( check_array( result, that, lookup(i, that.y) ) )
                break;

        for(var i=that.x+1; i<set_.Width; ++i)
            if( check_array( result, that, lookup(i, that.y) ) )
                break; 

        return result;
    }
    public function make_column(that: Square): Array{
        var result: Array = new Array();

        for(var i=that.y-1; i>=0; --i)
            if( check_array( result, that, lookup(that.x, i) ) )
                break;                                           

        for(var i=that.y+1; i<set_.Height; ++i)
            if( check_array( result, that, lookup(that.x, i) ) )
                break;  

        return result;
    }
    private function check_array(array: Array, that: Square, ya_that: Square): Boolean{
        if( ya_that == null ){
            return true;
        }
        else if( ya_that.rgb != that.rgb ){
            return true;
        }
        else if( ya_that.state instanceof Waiting ){
            array.push(ya_that);
			//trace("make_push (" + ya_that.x + "," + ya_that.y + ")")
            return false;
        }
        else{
            return true;
        }
    }
	
	//added
	public function tagCanClear(that: Square): Boolean {
		for( var x = that.x, y = that.y+1; y < set_.Height; ++y )
			if( lookup( x, y ) == null || lookup( x, y ).state instanceof Dying )
			    return false;
				
		return true;
	}

    public function isBelowEmpty(that: Square): Boolean{
        if( that.y+1 >= set_.Height ) return false;
        else return lookup(that.x, that.y+1) == null;
	}

    // getting/setting series
    public function lookup(x: Number, y: Number): Square{ return data_[y][x]; }
    public function setup(x: Number, y: Number, square: Square): Void{ data_[y][x] = square; }
    public function get body(): MovieClip{ return body_; }
    // delegation
    public function next_depth(): Number{ return game_.next_depth(); }
    // for Square's MovieClip to call...
    public function make_Dying(): Dying{ return game_.make_Dying(this); }
	public function make_Waiting(): Waiting{ return game_.make_Waiting(this); }
	//get data
	public function getData(): Array{ return data_; }
	//delete data
	public function delData(x: Number, y: Number){ data_[y][x] = null; }

    private function init_setting(setting: MapSetting): Void{
	
        set_ = setting;
        Width = set_.Width;
        Height = set_.Height;
        Size = set_.Size;
        Speed = set_.Speed;
        Frequency = set_.Frequency;
        Decay = set_.Decay;
        Delay = set_.Delay;

        Color_Amounts = set_.Color_Amounts;
        Chain_Amounts = set_.Chain_Amounts;

        Damage_Factor = set_.Damage_Factor;
    }

    private var body_: MovieClip;
    private var data_: Array;
    private var game_: Game;
    private var set_: MapSetting;
    private var step_: Number = 0;
    private var info_: TextField;
    private var score_: Number = 0;
    //private var chain_: Number = 0;      //will be deprecated
    //private var chain_tag_: Tag = null;  //will be deprecated
	
	//added
	public function id(): Number { return id_; }
	private var id_: Number = 0;
	private static var ID: Number = 0;

	//added
	public function num_of_chains(): Number { return taglist_.length }
	public function push_tag(tag: Tag): Number {
		return taglist_.push( tag );
	}
	public var taglist_: Array;         //this will keep the track of all the tags
	private var garbage_buffer_: Number;
	private var garbage_step_: Number;
	private var player_: Player;
	public function set_player(in_: Player): Void { player_ = in_; }
	public function player(): Player { return player_; }
	
	public var for_clone: Boolean;
}
