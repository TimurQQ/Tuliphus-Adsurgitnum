
#include "pch.h"
#include "Invader.h"

Invader::Invader()
	: Soldier()
{
	type_ = UnitType::Invader;
}

Invader::Invader(tmx::MapObject* relative_object, int max_health, int stripping_speed, int capture_speed)
	: Soldier(relative_object, max_health),
	stripping_speed_(stripping_speed),
	capture_speed_(capture_speed)
{
	type_ = UnitType::Invader;
}

bool Invader::Act() {
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
		if (needed_object_) {
			needed_position_ = needed_object_->position();
		}
		if (!MoveTo(needed_position_)) {
			state_ = SoldierState::Await;
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

void Invader::Interact(ResourcePoint* point) {
	if (point->colony() != my_colony_) {
		state_ = SoldierState::Capture;
		needed_object_ = point;
	}
}

void Invader::Interact(Soldier* soldier) {

}

void Invader::Interact(GameObject* g_obj) {

}
