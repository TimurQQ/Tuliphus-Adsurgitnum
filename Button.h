#pragma once
#include <SFML/Graphics/Text.hpp>
#include "Delegate.h"
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Button {
public:
	Button();

	const sf::Vector2f& position() const { return text_.getPosition(); }
	const sf::Vector2f& get_size() const { return size_; }

	const sf::String& GetString() const;

	void set_size(sf::Vector2f size) { size_ = size; }

	void SetText(std::string button_text);
	void SetPosition(float x, float y);

	void Draw(sf::RenderTarget* render_window);
private:
	sf::Vector2f size_; ///< Размер кнопки
	sf::Text text_; ///< Текст на кнопке
	sf::Font text_font_; ///< Шрифт текста

	void SetFont();
};