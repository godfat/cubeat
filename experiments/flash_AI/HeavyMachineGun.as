
class HeavyMachineGun extends Weapon
{
	public function HeavyMachineGun() {
		super();
		ammo_ = 30;
		max_ammo_ = 30;
		repeat_ = true;
		reloadtime_ = 100000;
		reloaddelay_ = reloadtime_;
		cooldowntime_ = 3;
		cooldowndelay_ = 0;
		reloadable_ = false;
		reloading_ = false;
		firepower_ = 1;
	}
	public function toString(): String { return "HeavyMachineGun"; }
}