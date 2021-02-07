#include "pch.h"
#include "Colony.h"
#include "Recruit.h"

Colony::Colony() {
	Colony::SetUnitsInfo();
}

void Colony::UpdateResource(ResourceType type, int value) {//Use for static addition 
	resources_[type] += value;
}

void Colony::UpdateResourceDelta(ResourceType type, int delta) {
	resource_delta_[type] += delta;
}

void Colony::AddSoldierOnLoad(tmx::MapObject* relative_object) {
	Soldier* newSoldier = new Recruit(relative_object);
	soldiers_.push_back(newSoldier);
}

Soldier* Colony::Update() {
	for (std::list<Soldier*>::iterator it = soldiers_.begin(); it != soldiers_.end(); ++it) {
		if (!(*it)->Act()) {
			Soldier* adress = *it;
			soldiers_.erase(it);
			return adress;
		}
	}
}

bool Colony::IsEnoughResources(UnitType unit_type) {
	auto& units_needed = base_.UnitsInfo();
	if (resources_[ResourceType::Food] >= consumed_food_ + units_needed[unit_type][ResourceType::Food] &&
		resources_[ResourceType::Acid] >= units_needed[unit_type][ResourceType::Acid] &&
		resources_[ResourceType::Salt] >= units_needed[unit_type][ResourceType::Salt])
		return true;
	return false;
}

void Colony::CreateNewUnit(UnitType unit_type) {
	if (!IsEnoughResources(unit_type)) {
		std::cout << "Not Enough Resources" << std::endl;
		return;
	}
	base_.CreateTransaction(unit_type);
	resources_ -= base_.UnitsInfo()[unit_type];
}

Button* Colony::GetActiveButton(sf::Vector2f mouse_pos) {
	Button* activeButton = nullptr;
	activeButton = base_.CheckButtons(mouse_pos);
	if (activeButton) {
		CreateNewUnit(unit_types_[activeButton->GetString()]);
	}
	return activeButton;
}

Soldier* Colony::FixedUpdate() {
	resources_ += resource_delta_;
	Soldier* newSoldier = nullptr;
	auto& units_needed = base_.UnitsInfo();
	if ((newSoldier = base_.ReturnUnit()) != nullptr) {
		soldiers_.push_front(newSoldier);
		consumed_food_ += units_needed[newSoldier->type()][ResourceType::Food];
	}
	return newSoldier;
}

Soldier* Colony::GetActiveSoldier(sf::Vector2f mouse_pos) {
	Soldier* activeSoldier = nullptr;
	for (auto soldier : soldiers_) {
		activeSoldier = soldier->WaitForInteraction(mouse_pos);
		if (activeSoldier) return activeSoldier;
	}
	return activeSoldier;
}

Base* Colony::GetActiveBase(sf::Vector2f world_mouse_pos) {
	if (base_.Ñontains(world_mouse_pos, sf::Vector2f(0.f, -96.f))) {
		return &base_;
	}
	return nullptr;
}

void Colony::Load(int id, tmx::MapObject* base_obj, int max_health) {
	id_ = id;
	base_.set_colony(this);
	base_.set_map_object(base_obj);
	base_.set_health(max_health);
	base_.SetButtonsInfo();
}

void Colony::DrawButtons(sf::RenderTarget* render_window) {
	base_.DrawButtons(render_window);
}

void Colony::SetUnitsInfo() {
	unit_types_["Recruit"] = UnitType::Recruit;
	unit_types_["AssaultSoldier"] = UnitType::AssaultSoldier;
	unit_types_["Invader"] = UnitType::Invader;
	unit_types_["Hunter"] = UnitType::Hunter;
	// ...
}

std::vector<int>& operator +=(std::vector<int>& a, std::vector<int>& b) {
	if (a.size() > b.size()) throw std::exception("LOL");
	for (int i = 0; i < a.size(); ++i) {
		a[i] += b[i];
	}
	return a;
}

std::vector<int>& operator -=(std::vector<int>& a, std::vector<int>& b) {
	if (a.size() > b.size()) throw std::exception("LOL");
	for (int i = 0; i < a.size(); ++i) {
		a[i] -= b[i];
	}
	return a;
}