#pragma once
#ifndef _LEVEL_H_INCLUDED_
#define _LEVEL_H_INCLUDED_

#include <vector>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <cassert>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <tmx/MapLoader.hpp>
#include <tmx/MapLayer.hpp>

#include "ResourcePoint.h"
#include "DynamicLayer.h"
#include "Colony.h"
#include "TableTemplateClass.h"

enum ObjectShapeOffset;

class Level {
public:
	Level(const std::vector<std::string>& sub_dirs, const std::string& mapFilename, sf::RenderWindow* render_window);
	~Level();

	void UpdateState(bool& show_debug);

	void Draw();
	void ShowDebug();
protected:
	// pass
private:
	sf::Clock time_;
	sf::Clock frame_clock_;
	sf::Text pinned_text_;
	sf::Font text_font_;
	std::map<UnitType, int /* gid */> get_GID_;
	std::map<std::string, ResourceType> get_ResType_;
	std::map<std::string, ObjectShapeOffset> get_ShapeOffset_;
	sf::RenderWindow* render_window_ = nullptr;
	Soldier* cur_unit_ = nullptr;
	sf::Event event_;
	tmx::MapLoader static_map_; // Прямоугольное поле, разделённое на клетки. Пустыня с лавовыми озёрами.
	DynamicLayer dynamic_objs_;
	std::vector<Colony> colonies_; // Восстановить колонии из баз (объектов типа база) на карте.
	Table<int /* id */, class ResourcePoint> resource_points_; // Восстановить ресурсные точки из объектов на карте.
	
	int GetBasesCount();

	void CreateDynamicLayer();

	void LoadStaticMap(const std::vector<std::string>& sub_dirs, const std::string& mapFilename);
	void LoadPinnedText();
	void LoadColonies();
	void LoadResourcePoints();

	void SetMapValues();
	void ReloadShapes();

	tmx::MapObject* AddMapObject(sf::Vector2f pos, sf::Vector2f size, float rot_angle, int gid = -1);

	void TryInteract(class Base* base);
	
	void TryInteract(class ResourcePoint* res_point);
	

	void TryAttack(Soldier* unit);
	
	void ProcessEvents(bool& show_debug);
	
	void ButtonPress(Button* button);
	void TryMoveCurUnit(sf::Vector2f world_mouse_pos);
	Soldier* GetUnitFromPos(sf::Vector2f mouse_pos);
	Button* GetButtonFromPos(sf::Vector2f mouse_pos);
	class ResourcePoint* GetResourcePointFromPos(sf::Vector2f mouse_pos);
	class Base* GetBaseFromPos(sf::Vector2f world_mouse_pos);
	void TryInteractWithPos(sf::Vector2f pos, Soldier*& new_unit, Button*& button, class ResourcePoint*& res_point, class Base*& base);
	void ProcessMouse(const sf::Mouse::Button& button);

	void UpdateColoniesState();
	void UpdateView();
	void UpdateText();
};

enum ObjectShapeOffset {
	Base = -96,
	Alien = 16,
	ResourcePoint = -32
};

#endif // _LEVEL_H_INCLUDED_