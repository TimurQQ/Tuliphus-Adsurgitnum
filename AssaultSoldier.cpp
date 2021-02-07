#include "pch.h"
#include "AssaultSoldier.h"

AssaultSoldier::AssaultSoldier()
	: Soldier() 
{
	type_ = UnitType::AssaultSoldier;
}

AssaultSoldier::AssaultSoldier(tmx::MapObject* relative_object, int max_health, int unit_damage, int base_damage)
	: Soldier(relative_object, max_health),
	unit_damage_(unit_damage),
	base_damage_(base_damage)
{
	type_ = UnitType::AssaultSoldier;
}

bool AssaultSoldier::Act() {
	switch (state_)
	{
	case Await:
		if (damageable_base_) {
			if (!Ñontains(damageable_base_->position())) {
				state_ = SoldierState::Find;
			}
			else {
				Interact(damageable_base_);
			}
		}
		else if (damageable_soldier_) {
			if (!Ñontains(damageable_soldier_->position())) {
				state_ = SoldierState::Find;
			}
			else {
				Interact(damageable_soldier_);
			}
		}
		else if (needed_object_) {
			needed_object_->Interact(this);
		}
		break;
	case Destroy:
		if (!damageable_base_->TakeDamage(unit_damage_)) {
			damageable_base_ = nullptr;
			damageable_soldier_ = nullptr;
			needed_object_ = nullptr;
			state_ = SoldierState::Find;
		}
		break;
	case Find:
		if (damageable_base_) {
			needed_object_ = damageable_base_;
		}
		else if (damageable_soldier_) {
			needed_object_ = damageable_soldier_;
		}
		if (needed_object_) {
			needed_position_ = needed_object_->position();
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

void AssaultSoldier::Interact(class Base* base) {
	if (base->colony() != my_colony_) {
		state_ = SoldierState::Destroy;
		needed_object_ = base;
	}
}

void AssaultSoldier::Interact(Soldier* soldier) {

}

void AssaultSoldier::Interact(GameObject* g_obj) {

}
