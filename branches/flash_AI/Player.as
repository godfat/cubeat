
class Player 
{
	public function Player(game: Game) {
		game_ = game;
		active_player_ = 1;
		trace( "player 1 activated" );
		
		weplist_ = new Array();
		weplist_.push( new Pistol() );
		weplist_.push( new HeavyPistol() );
		weplist_.push( new MachineGun() );
		weplist_.push( new HeavyMachineGun() );
		weplist_.push( new Rocket() );
		
		trace( weplist_ );
		
		current_wep_ = weplist_[0];  //pistol
	}
	
	public function cycle(): Void {
		process_input();
		
		if( changedelay_ > 0 ) --changedelay_;
		
		if( current_wep_.cycle() ) {
			//do nothing
		}
		else current_wep_ = weplist_[0];    //if the weapon is "DEAD" then we change it back to the most basic weapon
	}
	
	public function process_input(): Void {
		if( Key.isDown( 49 ) && weplist_[0] != null && current_wep_ != weplist_[0] ) {
			trace( "weapon pistol" );
			current_wep_ = weplist_[0];
			changedelay_ = changetime_;
		}
		else if( Key.isDown( 50 ) && weplist_[1] != null && current_wep_ != weplist_[1] ) {
			trace( "weapon heavy pistol" );
			current_wep_ = weplist_[1];
			changedelay_ = changetime_;
		}
		else if( Key.isDown( 51 ) && weplist_[2] != null && current_wep_ != weplist_[2] ) {
			trace( "weapon machine gun" );
			current_wep_ = weplist_[2];
			changedelay_ = changetime_;
		}
		else if( Key.isDown( 52 ) && weplist_[3] != null && current_wep_ != weplist_[3] ) {
			trace( "weapon heavy machine gun" );
			current_wep_ = weplist_[3];
			changedelay_ = changetime_;
		}
		else if( Key.isDown( 53 ) && weplist_[4] != null && current_wep_ != weplist_[4] ) {
			trace( "weapon rocket" );
			current_wep_ = weplist_[4];
			changedelay_ = changetime_;
		}
		
		if( Key.isDown( 90 ) ) current_wep_.reload();
	}
	
	public static function change_player(): Void {
		if( Key.isDown( 65 ) && active_player_ == 2 ) {  //A/a
			trace( "player 1 activated" );
			active_player_ = 1;
			Game.change_main_player_to( 0 );
		}
		else if( Key.isDown( 83 ) && active_player_ == 1 ) {   //S/s
			trace( "player 2 activated" );
			active_player_ = 2;
			Game.change_main_player_to( 1 );
		}
	}

	public static function active_player(): Number { return active_player_; }
	private static var active_player_: Number;
	
	//I'd better refactor this afterwards.
	public function add_weapon(type: Number): Void {
		switch( type ) {
			case 0:
				weplist_[1] = null;
				weplist_[1] = new HeavyPistol();
				if( current_wep_ instanceof HeavyPistol ) 
					current_wep_ = weplist_[1];
				break;
			case 1:
				weplist_[2] = null;
				weplist_[2] = new MachineGun();
				if( current_wep_ instanceof MachineGun ) 
					current_wep_ = weplist_[2];
				break;
			case 2:
				weplist_[3] = null;
				weplist_[3] = new HeavyMachineGun();
				if( current_wep_ instanceof HeavyMachineGun ) 
					current_wep_ = weplist_[3];
				break;
			case 3:
				weplist_[4] = null;
				weplist_[4] = new Rocket();
				if( current_wep_ instanceof Rocket ) 
					current_wep_ = weplist_[4];
				break;
		}
	}
	
	private var game_: Game;
	private var current_wep_: Weapon;
	private var weplist_: Array;
	private var changetime_: Number = 15;
	private var changedelay_: Number = 0;
	public function is_changing_wep(): Boolean { return changedelay_ > 0; }
	public function weapon(): Weapon { return current_wep_; }
}