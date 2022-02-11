#include "Button.h"

Button::Button(sf::Vector2f _pos, std::string _text, sf::Vector2f _size)
	:TextBox(_pos,_text,_size)
{
	auto textRect = box.getLocalBounds();
	box.setFillColor(sf::Color(250, 250, 250));
	text.setOrigin(_text.length()*3,6);
	text.setPosition(pos + sf::Vector2f(textRect.width / 2, textRect.height / 2));
	box.setSize(_size);
	clicked = false;
	clickTime = std::clock();
	centered = true;
}
void Button::draw(sf::RenderWindow* window)
{
	box.setPosition(pos);
	if (clicked)
	{
		box.setFillColor(sf::Color(150, 150, 150));
	}
	if ((std::clock() - clickTime) / (double)CLOCKS_PER_SEC > 0.5 && clicked)
	{
		box.setFillColor(sf::Color(250, 250, 250));
		clicked = false;
	}
	window->draw(box);
	window->draw(text);
}

void Button::click()
{
	clickTime = std::clock();
	clicked = true;
}