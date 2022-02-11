#pragma once
#include "BoxBase.h"
class TextBox
	:public BoxBase
{
protected:
	bool centered;
	sf::Text text;
public:
	TextBox(sf::Vector2f _pos, sf::String _text, sf::Vector2f _size);
	void setText(sf::String nText);
	sf::String getText();
	void setTextSize(int nSize);
	void setColor(sf::Color nColor);
	virtual void draw(sf::RenderWindow* window);
	void centerText();
	bool isInBounds(sf::Vector2i checked) const;
};

