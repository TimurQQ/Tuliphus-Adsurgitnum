#include "pch.h"
#include "GameObject.h"

GameObject::GameObject() {

}

GameObject::~GameObject() {

}

bool GameObject::Ñontains(const sf::Vector2f& point, const sf::Vector2f& offset, float eps) {
	sf::Vector2f size = map_object_->getSize();
	sf::Vector2f cornerPoint = map_object_->getPosition() + offset;
	if (point.x <= cornerPoint.x + size.x + eps &&
		point.x >= cornerPoint.x - eps &&
		point.y >= cornerPoint.y - eps &&
		point.y <= cornerPoint.y + size.y + eps) {
		return true;
	}
	return false;
}

bool GameObject::MoveTo(const sf::Vector2f& pos) {
	if (frame_clock_ == nullptr) {
		map_object_->setPosition(pos);
	}
	if (!Ñontains(pos)) {
		sf::Vector2f dpos = Normalized(pos - position());
		map_object_->move(dpos * velocity_ * frame_clock_->getElapsedTime().asSeconds());
		return true;
	}
	else return false;
}

GameObject* GameObject::WaitForInteraction(const sf::Vector2f& mouse_pos) {
	if (Ñontains(mouse_pos, offset_)) {
		return this;
	}
	return nullptr;
}

sf::Vector2f GameObject::Normalized(const sf::Vector2f a) {
	return a / sqrt(a.x * a.x + a.y * a.y);
}
