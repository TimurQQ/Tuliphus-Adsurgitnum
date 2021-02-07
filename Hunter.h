#pragma once
#ifndef _HUNTER_H_INCLUDED_
#define _HUNTER_H_INCLUDED_

#include <tmx/MapObject.hpp>
#include "Soldier.h"

class Hunter : public Soldier {
public:
	Hunter();

	Hunter(tmx::MapObject* relative_object, int max_health = 100, int unit_damage = 30);

	bool Act() override;

	int unit_damage() const { return unit_damage_; }

	void set_unit_damage(int damage) { unit_damage_ = damage; }

	virtual void Interact(Soldier* soldier) override;
	virtual void Interact(GameObject* g_obj) override;
private:
	int unit_damage_ = 30;
};

#endif // _HUNTER_H_INCLUDED_