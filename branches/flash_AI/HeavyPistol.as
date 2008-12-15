

class HeavyPistol extends Weapon
{
	public function HeavyPistol() {
		super();
		ammo_ = 6;
		max_ammo_ = 6;
		repeat_ = false;
		reloadtime_ = 100000;
		reloaddelay_ = reloadtime_;
		cooldowntime_ = 0;
		cooldowndelay_ = 0;
		reloadable_ = false;
		reloading_ = false;
		firepower_ = 3;
	}
	public function toString(): String { return "HeavyPistol"; }
}