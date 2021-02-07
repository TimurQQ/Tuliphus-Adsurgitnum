#include "pch.h"
#include "Level.h"

tmx::MapObject* Level::AddMapObject(sf::Vector2f pos, sf::Vector2f size, float rot_angle, int gid) {
	dynamic_objs_.AddObject(pos, size, rot_angle, gid);
	std::cout << pos.x << ' ' << pos.y << std::endl;
	return &dynamic_objs_.objects().front();
}

void Level::LoadStaticMap(const std::vector<std::string>& sub_dirs, const std::string& mapFilename) {
	for (const auto& sub_dir : sub_dirs) {
		static_map_.addSearchPath(sub_dir);
	}
	static_map_.load(mapFilename);
}

void Level::LoadPinnedText() {
	text_font_.loadFromFile("20051.ttf");
	pinned_text_.setFont(text_font_);
	pinned_text_.setPosition(sf::Vector2f(64.f, 64.f));
}

int Level::GetBasesCount() {
	int max_base_id = -1;
	for (tmx::MapLayer& layer : static_map_.getLayers()) {
		if (layer.type == tmx::MapLayerType::ObjectGroup) {
			for (tmx::MapObject& object : layer.objects) {
				if (object.getType() == "Base") {
					max_base_id = std::max(max_base_id, atoi(object.getPropertyString("colony_id").c_str()));
				}
			}
		}
	}
	return max_base_id;
}

void Level::LoadColonies() {
	//Заполнить массив colonies_
	for (tmx::MapLayer& layer : static_map_.getLayers()) {
		if (layer.type == tmx::MapLayerType::ObjectGroup) {
			for (tmx::MapObject& object : layer.objects) {
				if (object.getType() == "Base") {
					int id = atoi(object.getPropertyString("colony_id").c_str());
					int max_health = atoi(object.getPropertyString("max_health").c_str());
					//colonies_[id]
					colonies_[id].Load(id, &object, max_health);
				}
			}
		}
	}
}

void Level::LoadResourcePoints() {
	//Заполнить массив resource_points_
	for (tmx::MapLayer& layer : static_map_.getLayers()) {
		if (layer.type == tmx::MapLayerType::ObjectGroup) {
			for (tmx::MapObject& object : layer.objects) {
				if (object.getType() == "AcidLake" || object.getType() == "SaltCave" || object.getType() == "FoodPoint") {
					int id = atoi(object.getPropertyString("point_id").c_str());
					int max_resource_delta = atoi(object.getPropertyString("max_resource_delta").c_str());
					int max_num_workers = atoi(object.getPropertyString("max_num_workers").c_str());
					//resource_points_[id]
					resource_points_[id].Load(get_ResType_[object.getType()], id, &object, max_resource_delta, max_num_workers);
				}
			}
		}
	}
}

void Level::CreateDynamicLayer() {
	dynamic_objs_ = DynamicLayer(
		static_map_.width(),
		static_map_.height(),
		static_map_.tile_width(),
		static_map_.tile_height(),
		static_map_.patch_size(),
		static_map_.getTilesetTextures(),
		static_map_.getTileInfo(),
		render_window_
	);
}

void Level::ButtonPress(Button* button) {
	std::cout << "Button Pressed!!!!!!!!!" << "\n";
	for (auto& letter : button->GetString()) {
		std::cout << (char)letter;
	}
	std::cout << std::endl;
}

void Level::TryInteract(class Base* base) {
	cur_unit_->set_clock(&frame_clock_);
	cur_unit_->set_damageable(base);
}

void Level::TryInteract(class ResourcePoint* res_point) {
	if (resource_points_[res_point->get_id()].colony() == nullptr) {
		cur_unit_->set_clock(&frame_clock_);
		cur_unit_->set_needed_object(res_point);
		cur_unit_->set_state(SoldierState::Find);
		cur_unit_ = nullptr;
	}
	else if (resource_points_[res_point->get_id()].colony() != cur_unit_->get_colony() &&
		cur_unit_->type() != UnitType::AssaultSoldier
		)
	{
		cur_unit_->set_clock(&frame_clock_);
		cur_unit_->set_damageable(res_point);
	}
	else if (resource_points_[res_point->get_id()].num_workers()
		!=
		resource_points_[res_point->get_id()].max_num_workers())
	{
		cur_unit_->set_clock(&frame_clock_);
		cur_unit_->set_needed_object(res_point);
		cur_unit_->set_state(SoldierState::Find);
		cur_unit_ = nullptr;
	}
}

Soldier* Level::GetUnitFromPos(sf::Vector2f mouse_pos) {
	Soldier* unit = nullptr;
	for (auto& colony : colonies_) {
		unit = colony.GetActiveSoldier(mouse_pos);
		if (unit) return unit;
	}
	return unit;
}

Button* Level::GetButtonFromPos(sf::Vector2f mouse_pos) {
	Button* button = nullptr;
	for (auto& colony : colonies_) {
		button = colony.GetActiveButton(mouse_pos);
		if (button) return button;
	}
	return button;
}

class ResourcePoint* Level::GetResourcePointFromPos(sf::Vector2f mouse_pos) {
	class ResourcePoint* interacting_point = nullptr;
	for (auto& resource_point : resource_points_) {
		interacting_point = resource_point.value.WaitForInteraction(mouse_pos);
		if (interacting_point) return interacting_point;
	}
	return interacting_point;
}

class Base* Level::GetBaseFromPos(sf::Vector2f world_mouse_pos) {
	class Base* base_point = nullptr;
	for (auto& colony : colonies_) {
		base_point = colony.GetActiveBase(world_mouse_pos);
		if (base_point) return base_point;
	}
	return base_point;
}

void Level::TryInteractWithPos(sf::Vector2f pos, Soldier*& new_unit, Button*& button, class ResourcePoint*& res_point, class Base*& base) {
	if ((new_unit = GetUnitFromPos(pos)) != nullptr) {
		cur_unit_ = new_unit;
		cur_unit_->Detach(nullptr);
		std::cout << "I catch him!!!!!!!!!" << "\n";
	}
	else if ((button = GetButtonFromPos(pos)) != nullptr) {
		ButtonPress(button);
	}
	else if ((res_point = GetResourcePointFromPos(pos)) != nullptr) {
		std::cout << "Resource Point Detected!!!!!!!" << "\n";
		if (cur_unit_ && cur_unit_->type() != UnitType::Hunter) {
			TryInteract(res_point);
		}
	}
	else if ((base = GetBaseFromPos(pos)) != nullptr) {
		if (cur_unit_ && cur_unit_->type() == UnitType::AssaultSoldier) {
			TryInteract(base);
		}
	}
}

void Level::TryMoveCurUnit(sf::Vector2f world_mouse_pos) {
	if (cur_unit_) {
		cur_unit_->set_clock(&frame_clock_);
		cur_unit_->Detach(nullptr);
		cur_unit_->set_damageable((class ResourcePoint*)nullptr);
		cur_unit_->set_damageable((Soldier*)nullptr);
		cur_unit_->set_damageable((class Base*)nullptr);
		cur_unit_->set_needed_object(nullptr);
		cur_unit_->set_needed_position(world_mouse_pos);
		cur_unit_->set_state(SoldierState::Find);
	}
}

void Level::TryAttack(Soldier* damageable) {
	std::cout << "He will die!!!!!!!!!" << "\n";
	cur_unit_->set_clock(&frame_clock_);
	cur_unit_->set_damageable(damageable);
}

void Level::ProcessMouse(const sf::Mouse::Button& button) {
	
	sf::Vector2i pixel_mouse_pos = sf::Mouse::getPosition(*render_window_);
	sf::Vector2f world_mouse_pos = render_window_->mapPixelToCoords(pixel_mouse_pos);

	if (button == sf::Mouse::Left) {
		Soldier* new_unit = nullptr;
		Button* button = nullptr;
		class ResourcePoint* res_point = nullptr;
		class Base* base = nullptr;
		TryInteractWithPos(world_mouse_pos, new_unit, button, res_point, base);
	}
	if (button == sf::Mouse::Right) {
		Level::TryMoveCurUnit(world_mouse_pos);
	}
	if (button == sf::Mouse::Middle) {
		if (cur_unit_) {
			Soldier* damageable = nullptr;
			if ((damageable = GetUnitFromPos(world_mouse_pos)) != nullptr) {
				TryAttack(damageable);
			}
		}
	}
}

void Level::ProcessEvents(bool& show_debug) {
	while (render_window_->pollEvent(event_))
	{
		if (event_.type == sf::Event::Closed)
			render_window_->close();
		if (event_.type == sf::Event::KeyReleased)
		{
			if (event_.key.code == sf::Keyboard::D) {
				show_debug = !show_debug;
			}
		}
		if (event_.type == sf::Event::MouseButtonPressed) {
			sf::Mouse::Button button = event_.mouseButton.button;
			ProcessMouse(button);
		}
	}
}

void Level::UpdateColoniesState() {
	for (auto& colony : colonies_) {
		Soldier* dead_soldier = colony.Update();
		if (dead_soldier) {
			tmx::MapObject* adress = dead_soldier->get_map_object_ptr();
			adress->setVisible(false);
			for (std::list<tmx::MapObject>::iterator it = dynamic_objs_.objects().begin();
				it != dynamic_objs_.objects().end(); ++it)
			{
				if (&(*it) == adress) {
					dynamic_objs_.objects().erase(it);
					break;
				}
			}
			delete dead_soldier;
		}
	}
	if (time_.getElapsedTime().asSeconds() >= 1.f) {
		for (auto& colony : colonies_) {
			Soldier* newSoldier = colony.FixedUpdate();
			int gid = -1;
			if (newSoldier != nullptr) {
				gid = get_GID_[newSoldier->type()];
				newSoldier->set_map_object(
					AddMapObject(colony.get_base_pos(), sf::Vector2f(16.f, 16.f), 0.f, gid + 6 * colony.id())
				);
				newSoldier->set_clock(&frame_clock_);
				newSoldier->set_colony(&colony);
			}
		}
		time_.restart();
	}
}

void Level::UpdateView() {
	Level::UpdateText();
	sf::Vector2i pixel_mouse_pos = sf::Mouse::getPosition(*render_window_);
	sf::View view = render_window_->getView();
	sf::Vector2i size = (sf::Vector2i)render_window_->getSize();
	if (pixel_mouse_pos.x <= 0) {
		view.move(-1, 0);
		pinned_text_.move(-1, 0);
	}
	if (pixel_mouse_pos.x >= size.x - 1) {
		view.move(+1, 0);
		pinned_text_.move(+1, 0);
	}
	if (pixel_mouse_pos.y <= 0) {
		view.move(0, -1);
		pinned_text_.move(0, -1);
	}
	if (pixel_mouse_pos.y >= size.y - 1) {
		view.move(0, +1);
		pinned_text_.move(0, +1);
	}
	render_window_->setView(view);
}

void Level::UpdateText() {
	std::string base_ress = "";
	for (auto& colony : colonies_) {
		const std::vector<int>& ress = colony.resources();
		std::string newStr = "colony: " + std::to_string(colony.id()) + ' ' +
			"food: " + std::to_string(ress[ResourceType::Food]) + ' ' +
			"acid: " + std::to_string(ress[ResourceType::Acid]) + ' ' +
			"salt: " + std::to_string(ress[ResourceType::Salt]) + '\n';
		base_ress += newStr;
	}
	pinned_text_.setString(base_ress);
}

void Level::SetMapValues() {
	get_GID_[UnitType::Recruit] = 121;
	get_GID_[UnitType::AssaultSoldier] = 531;
	get_GID_[UnitType::Invader] = 555;
	get_GID_[UnitType::Hunter] = 507;
	get_ResType_["FoodPoint"] = ResourceType::Food;
	get_ResType_["AcidLake"] = ResourceType::Acid;
	get_ResType_["SaltCave"] = ResourceType::Salt;
	get_ShapeOffset_["Alien"] = ObjectShapeOffset::Alien;
	get_ShapeOffset_["Base"] = ObjectShapeOffset::Base;
	get_ShapeOffset_["AcidLake"] = ObjectShapeOffset::ResourcePoint;
	get_ShapeOffset_["SaltCave"] = ObjectShapeOffset::ResourcePoint;
}

void Level::ReloadShapes() {
	for (auto& layer : static_map_.getLayers()) {
		if (layer.type == tmx::MapLayerType::ObjectGroup) {
			for (tmx::MapObject& object : layer.objects) {
				DebugShape& debug_shape = object.debug_shape();
				sf::Color debugColour = sf::Color(127u, 127u, 127u);
				debugColour.a = static_cast<sf::Uint8>(255.f * object.visible());
				sf::Vector2f offset = sf::Vector2f(0.f, get_ShapeOffset_[object.getType()]);
				debug_shape.reset();
				debug_shape.open();
				for (sf::Vector2f& polypoint : object.polyPoints()) {
					polypoint += offset;
					debug_shape.addVertex(sf::Vertex(polypoint, debugColour));
				}
				debug_shape.closeShape();
				object.Calculate();
				object.createSegments();
			}
		}
	}
}
