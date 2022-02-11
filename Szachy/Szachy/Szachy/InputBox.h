#pragma once
#include "TextBox.h"
class InputBox :
	public TextBox
{
private:
	bool active;
public:
	void inputText(sf::Event event);
	void toggleActive();
	bool getActive();
	InputBox(sf::Vector2f _pos, std::string _text, sf::Vector2f _size);
	std::string getText();
	operator int() const;
	operator float() const;
};