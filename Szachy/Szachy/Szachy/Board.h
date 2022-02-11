#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Variables.h"

class Board
{
private:
	sf::Color bColor;
	sf::Color wColor;
	sf::Vector2i pos;
	sf::RectangleShape* tiles[8][8];
#ifdef DEBUG
	sf::Text* tileNumbers[8][8];
#endif // DEBUG
	sf::Font font;
public:
	Board(sf::Vector2i pos);
	virtual ~Board();

	void render(sf::RenderWindow* window);
	void setTileColor(int pos, sf::Color nColor);
	void setUnactive(int pos);
	void setActive(int pos);
};

