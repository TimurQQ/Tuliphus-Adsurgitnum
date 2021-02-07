#include "pch.h"
#include "Base.h"
#include "Recruit.h"
#include "Hunter.h"
#include "Invader.h"
#include "AssaultSoldier.h"

Base::Base(int max_health)
	: GameObject()
{
	health_ = max_health;
	Base::SetUnitsInfo();
}

Base::~Base() {
	transactions.clear();
}

void Base::CreateTransaction(UnitType unit_type) {
	transactions.push_back({ unit_type, sf::Clock() });
}

bool Base::IsMouseInButton(Button& button, sf::Vector2f mouse_pos) {
	const sf::Vector2f& size = button.get_size();
	sf::Vector2f cornerPoint = button.position();// +offset;
	if (mouse_pos.x <= cornerPoint.x + size.x &&
		mouse_pos.x >= cornerPoint.x &&
		mouse_pos.y >= cornerPoint.y &&
		mouse_pos.y <= cornerPoint.y + size.y) {
		return true;
	}
	return false;
}

Button*  Base::CheckButtons(sf::Vector2f mouse_pos) {
	Button* active_button = nullptr;
	for (auto& button : buttons_) {
		if (IsMouseInButton(button, mouse_pos)) {
			active_button = &button;
		}
	}
	return active_button;
}

Soldier* Base::ReturnUnit() {
	for (std::list<Transaction>::iterator it = transactions.begin(); it != transactions.end(); ++it) {
		float elapsed_time = it->clock.getElapsedTime().asSeconds();
		UnitType unit_type = it->unit_type;
		if (elapsed_time >= unit_needed_[unit_type][ResourceType::Time]) {
			Soldier* res = CreateUnit(unit_type);
			transactions.erase(it);
			return res;
		}
	}
	return nullptr;
}

void Base::DrawButtons(sf::RenderTarget* render_window) {
	for (auto& button : buttons_) {
		button.Draw(render_window);
	}
}

void Base::SetUnitsInfo() {
	unit_needed_[UnitType::Recruit] = {
		10,// food_needed
		10,// acid_needed
		10,// salt_needed
		5  // time_needed
	};
	unit_needed_[UnitType::AssaultSoldier] = {
		20,// food_needed
		20,// acid_needed
		20,// salt_needed
		10 // time_needed
	};
	unit_needed_[UnitType::Invader] = {
		15,// food_needed
		15,// acid_needed
		15,// salt_needed
		10 // time_needed
	};
	unit_needed_[UnitType::Hunter] = {
		30,// food_needed
		30,// acid_needed
		30,// salt_needed
		10 // time_needed
	};
}
void Base::SetButtonsInfo() {
	std::string names[] = {
		"Recruit", "AssaultSoldier",
		"Invader", "Hunter"
	};
	int length = sizeof(names) / sizeof(names[0]);
	buttons_ = std::vector<Button>(length);
	for (int i = 0; i < buttons_.size(); ++i) {
		buttons_[i].SetText(names[i]);
		sf::Vector2f button_size = buttons_[i].get_size();
		buttons_[i].SetPosition(position().x - button_size.x,
			position().y + i * button_size.y + 1
		);
	}
}

Soldier* Base::CreateUnit(UnitType type) {
	Soldier* newSoldier = nullptr;
	if (type == UnitType::Recruit) {
		newSoldier = new Recruit();
	}
	else if (type == UnitType::AssaultSoldier) {
		newSoldier = new AssaultSoldier();
	}
	else if (type == UnitType::Invader) {
		newSoldier = new Invader();
	}
	else if (type == UnitType::Hunter) {
		newSoldier = new Hunter();
	}
	return newSoldier;
}