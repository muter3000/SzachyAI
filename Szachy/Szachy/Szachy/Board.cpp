#include "Board.h"

Board::Board(sf::Vector2i pos)
	:bColor(125, 148, 93),wColor(238,238,213),pos(pos)
{
	font.loadFromFile("arial.ttf");
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			tiles[x][y] = new sf::RectangleShape(sf::Vector2f(tSize, tSize));
#ifdef DEBUG
			tileNumbers[x][y] = new sf::Text (std::to_string(63 - (x + y * 8)), font);
			tileNumbers[x][y]->setCharacterSize(30);
			tileNumbers[x][y]->setPosition(this->pos.x + tSize * x, this->pos.y + tSize * y);
#endif // DEBUG
			if ((y + x) % 2 == 0)
			{
				tiles[x][y]->setFillColor(wColor);
			}
			else
			{
				tiles[x][y]->setFillColor(bColor);
			}
			tiles[x][y]->setPosition(this->pos.x + tSize * x, this->pos.y + tSize * y);
		}
	}
}

Board::~Board()
{
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			delete tiles[x][y];
#ifdef DEBUG
			delete tileNumbers[x][y];
#endif
		}
	}
}

void Board::render(sf::RenderWindow* window)
{
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			window->draw(*tiles[x][y]);
		#ifdef DEBUG
			if (tileNumbers[x][y])
				window->draw(*tileNumbers[x][y]);
		#endif // DEBUG
		}
	}
}

void Board::setTileColor(int pos, sf::Color nColor)
{
	int x = pos % 8;
	int y = pos / 8;
	tiles[x][y]->setFillColor(nColor);
}

void Board::setUnactive(int pos)
{
	if (pos / 8 % 2 == 0)
	{
		(pos % 2 == 0) ? setTileColor(pos, wColor) : setTileColor(pos, bColor);
	}
	else
	{
		(pos % 2 != 0) ? setTileColor(pos, wColor) : setTileColor(pos, bColor);
	}
}

void Board::setActive(int pos)
{
	if (pos / 8 % 2 == 0)
	{
		(pos % 2 == 0) ? setTileColor(pos, sf::Color(233, 120, 120)) : setTileColor(pos, sf::Color(214, 1, 54));
	}
	else
	{
		(pos % 2 != 0) ? setTileColor(pos, sf::Color(233, 120, 120)) : setTileColor(pos, sf::Color(214, 1, 54));
	}
}


