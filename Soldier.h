#pragma once
#ifndef _SOLDIER_H_INCLUDED_
#define _SOLDIER_H_INCLUDED_

#include <vector>
#include <random>
#include <tmx/MapObject.hpp>
#include <SFML/System/Clock.hpp>
#include <cmath>
#include "GameObject.h"

enum class UnitType {
	Recruit,
	AssaultSoldier,
	Invader,
	Hunter
};

enum SoldierState {
	Await,
	Destroy,
	Capture,
	Find,
	Protect,
	Attack,
	GetResources,
	Dead
};

class Colony;

class Soldier : public GameObject {
public:
	Soldier();
	Soldier(tmx::MapObject* relative_object, int max_health = 100);

	double dodge_prob() const { return dodge_prob_; }
	UnitType type() { return type_; }
	Colony* get_colony() { return my_colony_; }
	SoldierState get_state() { return state_; }
	int health() { return health_; }

	void set_dodge_prob(double prob) { 
		dodge_prob_ = prob <=1.f && prob >=0.f ? prob : 0.f; 
	}
	void set_state(SoldierState state_type) { state_ = state_type; }
	virtual void set_needed_object(GameObject* object) { needed_object_ = object; }
	virtual void set_needed_position(sf::Vector2f pos) { needed_position_ = pos; }
	void set_colony(Colony* colony) { my_colony_ = colony; }
	void set_damageable(Soldier* soldier) { damageable_soldier_ = soldier; }
	void set_damageable(class Base* base) { damageable_base_ = base; }
	void set_damageable(class ResourcePoint* point) { damageable_point_ = point; }
	void set_health(int value) { 
		health_ = value > 0 ? value : 100;
	}

	virtual bool Act() = 0;

	virtual Soldier* WaitForInteraction(const sf::Vector2f& mouse_pos) override;

	bool TakeDamage(int damage);

	virtual void Detach(Soldier* soldier) override;
protected:
	Colony* my_colony_ = nullptr;
	SoldierState state_ = SoldierState::Await;
	GameObject* needed_object_ = nullptr;
	Soldier* damageable_soldier_ = nullptr;
	class Base* damageable_base_ = nullptr;
	class ResourcePoint* damageable_point_ = nullptr;
	sf::Vector2f needed_position_;
	int health_ = 100;
	double dodge_prob_ = 0.5; // Вероятность увернуться
	UnitType type_ = UnitType::Recruit;
	int gid_ = -1;
};

#endif // _SOLDIER_H_INCLUDED_