#pragma once
#ifndef _BASE_H_INCLUDED_
#define _BASE_H_INCLUDED_

#include <vector>
#include <set>
#include <list>
#include "TableTemplateClass.h"
#include "Soldier.h"
#include "Resource.h"
#include "Button.h"
#include <tmx/MapObject.hpp>
#include <map>
#include <SFML/System/Clock.hpp>
#include "GameObject.h"

struct Transaction {
	UnitType unit_type;
	sf::Clock clock;
};

class Soldier;

class Base : public GameObject {
public:
	Base(int max_health = 10000);
	~Base();

	int health() const { return health_; }
	Colony* colony() { return colony_; }
	Table<UnitType, std::vector<int>>& UnitsInfo() { return unit_needed_; }

	void set_health(int new_value) { health_ = new_value; }
	void set_colony(Colony* colony) { colony_ = colony; }

	void CreateTransaction(UnitType unit_type);
	Soldier* ReturnUnit();

	void SetButtonsInfo();
	void DrawButtons(sf::RenderTarget* render_window);
	Button* CheckButtons(sf::Vector2f mouse_pos);

	bool TakeDamage(int damage) {
		health_ -= damage;
		if (health_ < 0) {
			buttons_ = std::vector<Button>(0);
			return false;
		}
		std::cout << health_ << '\n';
		return true;
	}

	virtual void Detach(Soldier* soldier) {}

	virtual void Interact(Soldier* soldier) {}
	virtual void Interact(GameObject* g_obj) {}
protected:
	// pass
private:
	Colony* colony_ = nullptr;
	std::vector<Button> buttons_;
	int health_ = 10000; // Здоровье логова
	Table<UnitType, std::vector<int>> unit_needed_; // Таблица доступных юнитов для создания
	std::list<Transaction> transactions; // Таблица создаваемых юнитов

	void SetUnitsInfo();
	Soldier* CreateUnit(UnitType type);
	bool IsMouseInButton(Button& button, sf::Vector2f mouse_pos);
};

#endif // _BASE_H_INCLUDED_