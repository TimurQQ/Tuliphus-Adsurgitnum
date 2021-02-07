#include "pch.h"
#include "Button.h"

Button::Button() {
	Button::SetFont();
}

void Button::SetFont() {
	sf::Font text_font;
	text_font.loadFromFile("20051.ttf");
	text_font_ = text_font;
}

void Button::SetText(std::string button_text) {
	text_.setFont(text_font_);
	text_.setString(button_text);
	size_ = sf::Vector2f(
		text_.getGlobalBounds().width,
		text_.getGlobalBounds().height
	);
}

void Button::SetPosition(float x, float y) {
	text_.setPosition(x, y);
}

void Button::Draw(sf::RenderTarget* render_window) {
	render_window->draw(text_);
}

const sf::String& Button::GetString() const {
	return text_.getString();
}