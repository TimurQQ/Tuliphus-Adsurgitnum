
#include "pch.h"
#include "Hunter.h"

Hunter::Hunter()
	: Soldier()
{
	type_ = UnitType::Hunter;
}

Hunter::Hunter(tmx::MapObject* relative_object, int max_health, int unit_damage)
	: Soldier(relative_object, max_health)
{
	unit_damage_ = unit_damage > 0 ? unit_damage : 30;
	type_ = UnitType::Hunter;
}

bool Hunter::Act() {
	switch (state_)
	{
	case Await:
		if (damageable_soldier_) {
			if (!Ñontains(damageable_soldier_->position())) {
				state_ = SoldierState::Find;
			}
			else {
				Interact(damageable_soldier_);
			}
		}
		break;
	case Find:
		if (damageable_soldier_) {
			needed_position_ = damageable_soldier_->position();
		}
		if (!MoveTo(needed_position_)) {
			state_ = SoldierState::Await;
		}
		break;
	case Attack:
		if (!damageable_soldier_->TakeDamage(unit_damage_)) {
			damageable_soldier_ = nullptr;
			needed_object_ = nullptr;
			state_ = SoldierState::Find;
		}
		break;
	case Dead:
		return false;
		break;
	default:
		break;
	}
	return true;
}

void Hunter::Interact(Soldier* soldier) {
	if (soldier->get_colony() != my_colony_) {
		state_ = SoldierState::Attack;
		soldier->set_state(SoldierState::Attack);
		soldier->set_damageable(this);
		needed_object_ = soldier;
	}
}

void Hunter::Interact(GameObject* g_obj) {

}
