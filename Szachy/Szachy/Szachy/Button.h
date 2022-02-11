#pragma once
#include "TextBox.h"
class Button :
    public TextBox
{
private:
	bool clicked;
	std::time_t clickTime;
public:
	Button(sf::Vector2f _pos, std::string _text, sf::Vector2f _size);
	virtual void draw(sf::RenderWindow* window);
	void click();
};

