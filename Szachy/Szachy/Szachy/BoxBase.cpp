#include "BoxBase.h"

BoxBase::BoxBase(sf::Vector2f _pos, sf::Vector2f _size)
	:pos(_pos)
{
	font.loadFromFile("arial.ttf");
	box.setFillColor(sf::Color(150, 150, 150));
	box.setSize(_size);
	box.setPosition(pos);
}

BoxBase::~BoxBase()
{
}

void BoxBase::setPos(sf::Vector2f nPos)
{
	pos = nPos;
	box.setPosition(pos);
}

sf::Vector2f BoxBase::getPos()
{
	return pos;
}