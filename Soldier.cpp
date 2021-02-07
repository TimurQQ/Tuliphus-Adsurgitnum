#include "pch.h"
#include "Soldier.h"

Soldier::Soldier()
	: GameObject()
{}

Soldier::Soldier(tmx::MapObject* relative_object, int max_health)
	: GameObject()
{
	set_map_object(relative_object);
	health_ = max_health > 0 ? max_health : 100;
}

Soldier* Soldier::WaitForInteraction(const sf::Vector2f& mouse_pos) {
	if (Ñontains(mouse_pos, sf::Vector2f(0.f, 16.f))) {
		return this;
	}
	return nullptr;
}

bool Soldier::TakeDamage(int damage) {
	if (dodge_prob_ != 0.f) {
		if (std::rand() & 1) {
			health_ -= damage;
			if (health_ <= 0) {
				state_ = SoldierState::Dead;
				return false;
			}
		}
		return true;
	}
	else {
		health_ -= damage;
		if (health_ <= 0) {
			state_ = SoldierState::Dead;
			return false;
		}
		return true;
	}
}

void Soldier::Detach(Soldier* soldier) {
	if (needed_object_) {
		needed_object_->Detach(this);
		needed_object_ = nullptr;
	}
}