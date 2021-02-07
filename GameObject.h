#pragma once
#include <tmx/MapObject.hpp>
#include <SFML/System/Clock.hpp>

class Soldier;

/*!
	\brief ����� �������� �������

	������ ����� ����� ������������� �� �����. ��������� ��������
	�������� � ����������� ������ �������������� �������
*/
class GameObject {
public:
	GameObject();
	~GameObject();

	/*!
	\brief Returns pointer to relative map_object
	*/
	tmx::MapObject* get_map_object_ptr() { return map_object_; }
	/*!
	\brief Returns offset of the interactable field
	*/
	const sf::Vector2f& get_offset() const { return offset_; }
	/*!
	\brief Returns unique identifier of the object
	*/
	const int get_id() const { return id_; }
	/*!
	\brief Returns object position on map
	*/
	const sf::Vector2f& position() const { return map_object_->getPosition(); }

	/*!
	\brief Sets the relative map_object
	*/
	void set_map_object(tmx::MapObject* m_obj) { map_object_ = m_obj; }
	/*!
	\brief Set the time pointer to world time (using frame_clock)
	*/
	void set_clock(sf::Clock* frame_clock) { frame_clock_ = frame_clock; }
	/*!
	\brief Set the offset of the interactable field
	*/
	void set_offset(sf::Vector2f offset) { offset_ = offset; }
	/*!
	\brief Sets unique identifier of the object
	It is recommended to use unique identidiers
	*/
	void set_id(const int id) { id_ = id; }

	/*!
		\brief Check if the needed point  ��������� ����� �� ������ ����� ������ �������� �������
		\param[in] point ����������� �����
		\param[in] offset �������� ������������� ����� ������� 
		\param[in] eps �������� ����������
		\return ���������� ��������� ��������, ���������� ��������� ����� � ������
	*/
	bool �ontains(const sf::Vector2f& point, const sf::Vector2f& offset = sf::Vector2f(0.f, 0.f), float eps = 0.1f);

	/*!
		\brief ���������� ������ � ������ �������
		\param[in] pos ������ �������
		\return ����� ���� �����, ���������� � �������� ����������
	*/
	bool MoveTo(const sf::Vector2f& pos);

	virtual void Interact(GameObject* g_obj) = 0;
	virtual void Interact(Soldier* g_obj) = 0;

	virtual void Detach(Soldier* soldier) = 0;

	virtual GameObject* WaitForInteraction(const sf::Vector2f& mouse_pos);
protected:
	// pass
private:
	float velocity_ = 60.f;
	tmx::MapObject* map_object_ = nullptr;
	sf::Clock* frame_clock_ = nullptr;
	sf::Vector2f offset_;
	int id_ = -1;

	sf::Vector2f Normalized(const sf::Vector2f a);
};