#pragma once
#ifndef _RECRUIT_H_INCLUDED_
#define _RECRUIT_H_INCLUDED_

#include <tmx/MapObject.hpp>
#include "ResourcePoint.h"

class Recruit : public Soldier{
public:
	Recruit();

	Recruit(tmx::MapObject* relative_object, int max_health = 100, int unit_damage = 10, int capture_speed = 10);

	bool Act() override;

	int unit_damage() const { return unit_damage_; }
	int capture_speed() const { return capture_speed_; }

	void set_unit_damage(int damage) { unit_damage_ = damage; }
	void set_capture_speed(int speed) { capture_speed_ = speed; }

	void Interact(ResourcePoint* point);
	void Interact(Soldier* soldier) override;
	void Interact(GameObject* g_obj) override;
private:
	int unit_damage_ = 10;
	int capture_speed_ = 10;
};

#endif // _RECRUIT_H_INCLUDED_