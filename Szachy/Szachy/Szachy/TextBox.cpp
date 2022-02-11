#include "TextBox.h"

TextBox::TextBox(sf::Vector2f _pos, sf::String _text,sf::Vector2f _size)
	:BoxBase(_pos,_size)
{
	centered = false;
	text.setFont(font);
	text.setCharacterSize(11);
	text.setFillColor(sf::Color::Black);
	text.setString(_text);
	text.setPosition(pos + sf::Vector2f(5,2));
}
void TextBox::setText(sf::String nText)
{ 
	text.setString(nText);
}

sf::String TextBox::getText()
{
	return text.getString();
}

void TextBox::setTextSize(int nSize)
{
	text.setCharacterSize(nSize);
}

void TextBox::setColor(sf::Color nColor)
{
	box.setFillColor(nColor);
}

void TextBox::draw(sf::RenderWindow* window)
{
	box.setPosition(pos);
	if (centered)
		text.setPosition(pos + sf::Vector2f(box.getLocalBounds().width / 2, box.getLocalBounds().height / 2));
	window->draw(box);
	window->draw(text);
}

void TextBox::centerText()
{
	sf::FloatRect lBounds = text.getLocalBounds();
	text.setOrigin(lBounds.left + lBounds.width / 2, lBounds.top + lBounds.height / 2);
	centered = true;
}

bool TextBox::isInBounds(sf::Vector2i checked) const
{
	checked -= (sf::Vector2i)box.getPosition();
	return(checked.x > 0 && checked.y > 0 && checked.x < box.getLocalBounds().width&& checked.y < box.getLocalBounds().height);
}
