#pragma once
#ifndef _INVADER_H_INCLUDED_
#define _INVADER_H_INCLUDED_

#include <tmx/MapObject.hpp>
#include "Soldier.h"
#include "ResourcePoint.h"

class Invader : public Soldier {
public:
	Invader();

	Invader(tmx::MapObject* relative_object, int max_health = 100, int stripping_speed = 15, int capture_speed = 15);

	bool Act() override;

	int capture_speed() const { return capture_speed_; }
	int stripping_speed() const { return stripping_speed_; }

	void set_capture_speed(int speed) { capture_speed_ = speed; }
	void set_stripping_speed(int speed) { stripping_speed_ = speed; }

	void Interact(ResourcePoint* point);
	void Interact(Soldier* soldier) override;
	void Interact(GameObject* g_obj) override;
private:
	int capture_speed_ = 15;
	int stripping_speed_ = 15;
};

#endif // _INVADER_H_INCLUDED_