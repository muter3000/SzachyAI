#include "InputBox.h"

InputBox::InputBox(sf::Vector2f _pos, std::string _text, sf::Vector2f _size)
	:TextBox(_pos,_text,_size)
{
	active = false;
	this->box.setFillColor(sf::Color(200, 200, 200));
}

void InputBox::inputText(sf::Event event)
{

	std::string pText = text.getString();
	if (event.text.unicode == 8)
	{
		std::string nText = pText;
		nText.resize(pText.length()==0?0:pText.length() - 1);
		text.setString(nText);
	}
	else if ((47 <= event.text.unicode && event.text.unicode < 122)|| event.text.unicode==32)
	{
		pText += static_cast<char>(event.text.unicode);
		text.setString(pText);
	}
}

void InputBox::toggleActive()
{
	if (!active)
	{
		box.setFillColor(box.getFillColor() + sf::Color(50, 50, 50));
		active = true;
	}
	else
	{
		box.setFillColor(box.getFillColor() - sf::Color(50, 50, 50, 0));
		active = false;
	}
}

bool InputBox::getActive()
{
	return active;
}

std::string InputBox::getText()
{
	return text.getString();
}

InputBox::operator int() const
{
	return std::stoi((std::string)text.getString());
}

InputBox::operator float() const
{
	return std::stof((std::string)text.getString());
}
