#pragma once
#ifndef _COLONY_H_INCLUDED_
#define _COLONY_H_INCLUDED_

#include "Base.h"
#include "Soldier.h"
#include <map>
#include "Resource.h"
#include <SFML/Window/Window.hpp>

class Colony {
public:
	Colony();

	const int id() const { return id_; }
	std::vector<int>& resources() { return resources_; }
	const sf::Vector2f& get_base_pos() const { return base_.position(); }

	Soldier* Update();
	Soldier* FixedUpdate();
	//Use for static addition
	void UpdateResource(ResourceType type, int value);
	void UpdateResourceDelta(ResourceType type, int delta);

	Soldier* GetActiveSoldier(sf::Vector2f mouse_pos);
	Button* GetActiveButton(sf::Vector2f mouse_pos);
	class Base* GetActiveBase(sf::Vector2f world_mouse_pos);

	void CreateNewUnit(UnitType unit_type);

	void Load(int id, tmx::MapObject* base_obj, int max_health);
	void AddSoldierOnLoad(tmx::MapObject* relative_object);

	void DrawButtons(sf::RenderTarget* render_window);
protected:
	// pass
private:
	Table<std::string, UnitType> unit_types_;
	std::list<Soldier*> soldiers_; // Список солдат колонии
	class Base base_; // Запись логова колонии
	int consumed_food_ = 0;
	std::vector<int> resources_ = {250, 125, 50}; // Текущие запасы ресурсов 0-Пища, 1-Кислота, 2-Соль
	std::vector<int> resource_delta_ = {0, 0, 0}; // Прибиль пищи, солей и кислоты
	int id_ = -1; // Номер колонии

	void SetUnitsInfo();
	bool IsEnoughResources(UnitType unit_type);
};

std::vector<int>& operator +=(std::vector<int>& a, std::vector<int>& b);
std::vector<int>& operator -=(std::vector<int>& a, std::vector<int>& b);

#endif // _COLONY_H_INCLUDED_