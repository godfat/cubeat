import mx.utils.Delegate;

class Game{
    public function Game(){
        body_ = _root.createEmptyMovieClip("game", depth_);
        setting_ = new MapSetting();            //changed to class member
        maps_.push( new Map(this, setting_, 0, 0) );
        maps_.push( new Map(this, setting_, setting_.Size*(setting_.Width+1), 0) );

        maps_[0].move_info_board(300, 0);
        maps_[1].move_info_board(300, 400);
		
		//added
		players_.push( new Player(this) );
		players_.push( new Player(this) );
		main_player_ = players_[0];
		maps_[0].set_player( players_[0] );
		maps_[1].set_player( players_[1] );
		itembox_ = null;
    }

    public function run(){
        body_.self = this;
        /*for(var i in maps_){
            for(var y=maps_[i].Height-1, y_end = Math.floor(maps_[i].Height/2); y>=y_end; --y){
                for(var x=0; x<maps_[i].Width; ++x){
                    new Square(maps_[i], x, y, Math.floor(Math.random()*4));
                }
            }
        }*/
		
		//new AI
		Ai_ = new Ai(this,maps_[0]);
		setInterval( Delegate.create(Ai_, Ai_.travelMap), 4000, maps_[0]);

		var testcase: Array = [[9,9,9,9,9,9],
							   [9,9,9,9,9,9],
							   [9,9,9,9,9,9],
							   [9,9,9,9,9,9],
							   [9,0,9,9,9,9],
							   [9,1,3,9,9,9],
							   [9,1,2,1,9,9],
							   [9,2,2,1,9,9],
							   [0,1,0,0,9,9],
							   [1,3,2,3,3,9],
							   [1,2,0,0,3,9]];
		
		for( var i = testcase.length -1; i >= 0; i-- ) {
			for( var j = testcase[i].length -1; j >= 0 ; j-- ) {
				if( testcase[i][j] != 9 )
					new Square(maps_[0], j, i, testcase[i][j]);
			}
		}
		
        body_.onEnterFrame = cycle;
		
		//added
		body_.onMouseMove = myMouseMove;
		body_.onMouseDown = myMouseDown;
		body_.onMouseUp = myMouseUp;
    }
	
	//added
	private function myMouseMove(): Void {  //deal with the cursor movements here
	}
	
	//added
	private function myMouseUp(): Void {
		self.trigger_ = false;
	}
	
	//added: single shot
	private function myMouseDown(): Void {  //deal with the real input here
		if( Game.main_player_.is_changing_wep() ) return;
		if( Game.main_player_.weapon().can_fire() == false ) return;   

		self.trigger_ = true;
		Game.main_player_.weapon().fire();
		
		if( self.confirm_hit_item() ) return;
		
		if( self.maps_[0].body._xmouse < 300 ) {    //hit on map 1
			self.confirm_hit_map( self.maps_[0] );
		}
		else if ( self.maps_[1].body._xmouse >= 0 ) {  //hit on map 2
			self.confirm_hit_map( self.maps_[1] );
		}
	}
	
	//added: my own hit test part 1
	private function confirm_hit_map( tempmap: Map ): Void {
		if( Game.main_player_.weapon() instanceof Rocket ) {
			for( var i = -1; i <= 1; ++i ) {        //iterate through y
				for( var j = -1; j <= 1; ++j ) {    //iterate through x
					confirm_hit_square( tempmap, 
									    tempmap.body._xmouse + j * setting_.Size,
										tempmap.body._ymouse + i * setting_.Size );
				}
			}
		}
		else {
			confirm_hit_square( tempmap, 
								tempmap.body._xmouse,
								tempmap.body._ymouse );
		}		
	}
	
	//added: my own hit test part 2
	private function confirm_hit_square( tempmap: Map, tempx: Number, tempy: Number ): Void {
		var logicx: Number = Math.floor( tempx / setting_.Size );
		var logicy: Number = Math.floor( tempy / setting_.Size );
		var cube: Square = null;

		if( (cube = tempmap.lookup( logicx, logicy )) != null ) { //the cube is logically there
			if( tempy - cube.body._y < setting_.Size ) //hit!
				cube.i_am_hit( main_player_.weapon().firepower() );
		}
		if( (cube = tempmap.lookup( logicx, logicy+1 )) != null ) { //the cube is logically there
			if( cube.body._y < tempy ) //hit!
				cube.i_am_hit( main_player_.weapon().firepower() );
		}
	}
	
	//added: continuous firing
	private function process_input_trigger(): Void {
		if( Game.main_player_.weapon().can_fire() == false || 
			Game.main_player_.weapon().can_fire_repeatedly() == false ) return; 
		
		Game.main_player_.weapon().fire();
		
		if( confirm_hit_item() ) return;
		
		if( maps_[0].body._xmouse < 300 ) {    //hit on map 1
			confirm_hit_map( maps_[0] );
		}
		else if ( maps_[1].body._xmouse >= 0 ) {  //hit on map 2
			confirm_hit_map( maps_[1] );
		}
	}
	
	private function confirm_hit_item(): Boolean {
		if( itembox_ == null ) return false;
		if( itembox_.body.hitTest( body._xmouse, body._ymouse, 1 ) ) {
			Game.main_player_.add_weapon( itembox_.item_type() );
			itembox_.i_am_hit();
			return true;
		}
		return false;
	}

    // making series
    public function make_Dropping(map: Map, that: Square): Dropping{ return new Dropping(map); }
    public function make_Waiting(map: Map): Waiting{ return new Waiting(map); }
    public function make_Dying(map: Map): Dying{ return new Dying(map); }
    public function make_garbage(map: Map, amounts: Number): Void{
        for(var i in maps_){
            if( maps_[i] != map ){
                maps_[i].make_garbage(amounts);
            }
        }
    }
    public function next_depth(): Number{ return ++depth_; }

    // getting series
    public function get body(): MovieClip{ return body_; }

    private function cycle(): Void{
		
		if( self.trigger_ == true ) self.process_input_trigger();   //add
		
        // what a wonderful for in loop that could only get the index of an container
        for(var i in self.maps_)
            self.maps_[i].cycle();
			
		//added: Item Box
		self.itembox_create();
		if( self.itembox_ != null ) {
			self.itembox_.cycle();
		}
					
		//added
		Player.change_player();
		for(var i in Game.players_)
			if( i == (Player.active_player()-1) )   //a stupid mistake here... i is (0 and 1) but player_id is (1 and 2) ...
				Game.players_[i].cycle();
				
    }

	//added
	private function itembox_create(): Void {
		if( ++step_ > setting_.Frequency * 15 ){
			if( itembox_ != null ) itembox_.suicide();
			itembox_ = new Item( this, Math.floor( Math.random()*2 ) );			           
            step_ = 0;
        }
	}

    private var dropping_: Dropping;
    // private var waiting_: Waiting;
    private var dying_: Dying;

    private var depth_: Number = 10;
    private var body_: MovieClip;
    private var maps_: Array = new Array();
	
	//added
	public static function change_main_player_to(i: Number): Void { 
		if( players_[i] != null )
			main_player_ = players_[i]; 
	}
	
	private static var main_player_: Player;
	private static var players_: Array = new Array();
	private var trigger_: Boolean = false;
	private var setting_: MapSetting;
	private var step_: Number = 0;
	private var itembox_: Item;
	private var Ai_: Ai;
	// dummy
    private var self: Game;
}
