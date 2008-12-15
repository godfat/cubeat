
class Weapon
{
	public function Weapon() {
		ammo_ = 0;
		max_ammo_ = 0;
		repeat_ = false;
		reloadtime_ = 15;
		reloaddelay_ = reloadtime_;
		cooldowntime_ = 0;
		cooldowndelay_ = 0;
		reloadable_ = true;
		reloading_ = false;
		firepower_ = 0;
	}
	
	public function fire(): Void {       //trigger fire
		if( ammo_ > 0 && reloading_ == false ) {
			--ammo_;
			cooldowndelay_ = cooldowntime_;
		}
	}
	
	public function reload(): Void {     //trigger reload
		if( reloadable_ == true ) 
			reloading_ = true;
	}
	
	public function cycle(): Boolean {        //delay cycle
		if( reloading_ == true ) {
			if( reloaddelay_ > 0 ) {
				--reloaddelay_;
			}
			else {
				reloading_ = false;
				ammo_ = max_ammo_;
				reloaddelay_ = reloadtime_;
			}
		}
		
		if( cooldowndelay_ > 0 ) --cooldowndelay_;
		
		if( reloadable_ == false && ammo_ == 0 ) return false;
		else return true;
	}
	
	public function firepower(): Number { return firepower_; }
	public function reloadable(): Boolean { return reloadable_; }
	public function delaytime(): Number { return reloaddelay_; }
	public function is_reloading(): Boolean { return reloading_; }
	public function is_coolingdown(): Boolean { return cooldowndelay_ > 0; }
	public function ammo(): Number { return ammo_; }
	public function max_ammo(): Number { return max_ammo_; }
	public function toString(): String { return "Something Wrong."; }
	public function can_fire(): Boolean { return !is_reloading() && !is_coolingdown() && ammo() > 0; }
	public function can_fire_repeatedly(): Boolean { return repeat_; }
	
	private var ammo_: Number;
	private var max_ammo_: Number;
	private var reloadable_: Boolean;
	private var reloading_: Boolean;
	private var reloadtime_: Number;
	private var reloaddelay_: Number;
	private var firepower_: Number;
	private var repeat_: Boolean;
	private var cooldowntime_: Number;
	private var cooldowndelay_: Number;
}