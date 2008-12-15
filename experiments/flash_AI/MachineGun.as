
class MachineGun extends Weapon
{
	public function MachineGun() {
		super();
		ammo_ = 15;
		max_ammo_ = 15;
		repeat_ = true;
		reloadtime_ = 30;
		reloaddelay_ = reloadtime_;
		cooldowntime_ = 3;
		cooldowndelay_ = 0;
		reloadable_ = true;
		reloading_ = false;
		firepower_ = 1;
	}
	public function toString(): String { return "MachineGun"; }
}