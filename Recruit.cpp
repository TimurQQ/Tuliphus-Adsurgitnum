#include "pch.h"
#include "Recruit.h"

Recruit::Recruit()
	: Soldier()
{
	type_ = UnitType::Recruit;
}

Recruit::Recruit(tmx::MapObject* relative_object, int max_health, int unit_damage, int capture_speed)
	: Soldier(relative_object, max_health),
	unit_damage_(unit_damage),
	capture_speed_(capture_speed)
{
	type_ = UnitType::Recruit;
}

bool Recruit::Act() {
	switch (state_)
	{
	case Await:
		if (damageable_point_) {
			if (!Ñontains(damageable_point_->position())) {
				state_ = SoldierState::Find;
			}
			else {
				Interact(damageable_point_);
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
	case Capture:
		if (!damageable_point_->TakeDamage(capture_speed_)) {
			damageable_point_->Interact(this);
			damageable_point_ = nullptr;
			damageable_soldier_ = nullptr;
			needed_object_ = nullptr;
			state_ = SoldierState::Find;
		}
	case Find:
		if (damageable_point_) {
			needed_object_ = damageable_point_;
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
		if (damageable_soldier_ && !damageable_soldier_->TakeDamage(unit_damage_)) {
			damageable_soldier_ = nullptr;
			needed_object_ = nullptr;
			state_ = SoldierState::Find;
		}
		break;
	case GetResources:
		// pass
		break;
	case Dead:
		return false;
		break;
	default:
		break;
	}
	return true;
}

void Recruit::Interact(ResourcePoint* point) {
	if (point->colony() != my_colony_) {
		state_ = SoldierState::Capture;
		needed_object_ = point;
	}
}

void Recruit::Interact(Soldier* soldier) {
	if (soldier->get_colony() != my_colony_) {
		state_ = SoldierState::Attack;
		soldier->set_state(SoldierState::Attack);
		soldier->set_damageable(this);
		needed_object_ = soldier;
	}
}

void Recruit::Interact(GameObject* g_obj) {

}