
class Pistol extends Weapon
{
	public function Pistol() {
		super();
		ammo_ = 9999;
		max_ammo_ = 9999;
		repeat_ = false;
		reloadtime_ = 15;
		reloaddelay_ = reloadtime_;
		cooldowntime_ = 0;
		cooldowndelay_ = 0;
		reloadable_ = true;
		reloading_ = false;
		firepower_ = 1;
	}
	public function toString(): String { return "Pistol"; }
}