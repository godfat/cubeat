
class Rocket extends Weapon
{
	public function Rocket() {
		super();
		ammo_ = 1;
		max_ammo_ = 1;
		repeat_ = false;
		reloadtime_ = 100000;
		reloaddelay_ = reloadtime_;
		cooldowntime_ = 0;
		cooldowndelay_ = 0;
		reloadable_ = false;
		reloading_ = false;
		firepower_ = 3;
	}
	public function toString(): String { return "Rocket"; }
}