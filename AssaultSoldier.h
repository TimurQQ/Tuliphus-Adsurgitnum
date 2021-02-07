#pragma once
#ifndef _ASSAULT_SOLDIER_H_INCLUDED_
#define _ASSAULT_SOLDIER_H_INCLUDED_

#include <tmx/MapObject.hpp>
#include "Base.h"
#include "Soldier.h"

class AssaultSoldier : public Soldier {
public:
	AssaultSoldier();

	AssaultSoldier(tmx::MapObject* relative_object, int max_health = 100, int unit_damage = 20, int base_damage = 20);

	bool Act() override;

	const int base_damage() const { return base_damage_; }
	const int unit_damage() const { return unit_damage_; }

	void set_base_damage(int damage) { base_damage_ = damage; }
	void set_unit_damage(int damage) { unit_damage_ = damage; }

	void Interact(class Base* base);
	virtual void Interact(Soldier* g_obj);
	virtual void Interact(GameObject* g_obj);
private:
	int unit_damage_ = 20;
	int base_damage_ = 20;
};

#endif // _ASSAULT_SOLDIER_H_INCLUDED_