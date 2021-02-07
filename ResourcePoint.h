#pragma once
#ifndef _RESOURCE_POINT_H_INCLUDED_
#define _RESOURCE_POINT_H_INCLUDED_

#include <tmx/MapObject.hpp>
#include "GameObject.h"
#include "Colony.h"
#include "Resource.h"

class ResourcePoint : public GameObject {
public:
	ResourcePoint()
		: GameObject()
	{}

	ResourcePoint(int max_resource_delta, int max_num_workers)
		: GameObject(),
		max_resource_delta_(max_resource_delta),
		max_num_workers_(max_num_workers)
	{}
	
	~ResourcePoint() {
		workers.clear();
	}

	int resource_delta() { return resource_delta_; }
	int max_resource_delta() const { return max_resource_delta_; }
	int num_workers() const { return num_workers_; }
	int max_num_workers() const { return max_num_workers_; }
	Colony* colony() { return colony_; }
	ResourceType type() { return type_; }

	void set_resource_delta(int resource_delta) { resource_delta_ = resource_delta; }
	void set_max_resource_delta(int new_value) { max_resource_delta_ = new_value; }
	void set_num_workers(int num_workers) { num_workers_ = num_workers; }
	void set_max_num_workers(int num_workers) { max_num_workers_ = num_workers; }
	void set_colony(Colony* colony) { colony_ = colony; }
	void set_resource_type(ResourceType type) { type_ = type; }

	virtual ResourcePoint* WaitForInteraction(sf::Vector2f& mouse_pos) {
		if (Сontains(mouse_pos, sf::Vector2f(0.f, -32.f))) {
			return this;
		}
		return nullptr;
	}

	void Load(ResourceType type, int id, tmx::MapObject* m_obj, int max_resource_delta, int max_num_workers) {
		set_resource_type(type);
		set_id(id);
		set_map_object(m_obj);
		set_max_resource_delta(max_resource_delta);
		set_max_num_workers(max_num_workers);
	}

	bool TakeDamage(int damage) {
		// Зачистка
		to_strip -= damage;
		if (to_strip < 0) {
			return false;
		}
		return true;
	}

	virtual void Interact(Soldier* soldier) override {
		if (soldier->type() == UnitType::Recruit) {
			// Забрать в рабство
			if (num_workers_ < max_num_workers_) {
				if (!colony_ || colony_ != soldier->get_colony()) {
					colony_ = soldier->get_colony();
				}
				soldier->set_state(SoldierState::GetResources);
				int delta = 1 * max_resource_delta_ / max_num_workers_;
				num_workers_++;
				workers.push_back(soldier);
				to_strip += 1000;
				resource_delta_ += delta;
				colony_->UpdateResourceDelta(type_, delta);
			}
		}
		else if (soldier->type() == UnitType::Invader) {
			colony_->UpdateResourceDelta(type_, -resource_delta_);
			for (auto* cur_unit : workers) {
				cur_unit->set_damageable((class ResourcePoint*)nullptr);
				cur_unit->set_damageable((Soldier*)nullptr);
				cur_unit->set_damageable((class Base*)nullptr);
				cur_unit->set_needed_object(nullptr);
				cur_unit->set_needed_position(cur_unit->get_colony()->get_base_pos());
				cur_unit->set_state(SoldierState::Find);
			}
			workers.clear();
			colony_ = soldier->get_colony();
			resource_delta_ = 0;
			num_workers_ = 0;
		}
	}

	virtual void Detach(Soldier* soldier) {
		for (std::list<Soldier*>::iterator it = workers.begin(); it != workers.end(); ++it) {
			if (*it == soldier) {
				num_workers_--;
				int delta = 1 * max_resource_delta_ / max_num_workers_;
				resource_delta_ -= delta;
				colony_->UpdateResourceDelta(type_, -delta);
				workers.erase(it);
				return;
			}
		}
	}

	virtual void Interact(GameObject* g_obj) override {}

protected:
	//pass
private:
	int to_capture = 1000;
	int to_strip = 0;
	std::list<Soldier*> workers;
	class Colony* colony_ = nullptr; // Принадлежность к колонии (nullptr == нейтральная)
	ResourceType type_ = ResourceType::Food; // Тип ресурса, добываемого в ресурсной точке
	int resource_delta_ = 0; // Текущая прибыль в секунду
	int max_resource_delta_ = 0; // Максимальная прибыль в секунду
	int num_workers_ = 0; // Текущий уровень заселения
	int max_num_workers_ = 0; // Максимальный уровень заселения
};

#endif // _RESOURCE_POINT_H_INCLUDED_