#include "Piece.h"
#include <iostream>
Piece::Piece(sf::Vector2i pos)
	:pos(pos),color(false),sprite(nullptr),pieceType(PieceType::bPawn)
{
}

Piece::~Piece()
{
	delete sprite;
}

sf::Vector2i Piece::getPos()
{
	return pos;
}

int Piece::getPosInt()
{
	return pos.x+pos.y*8;
}

void Piece::updatePosLive(sf::Vector2i nPos)
{
	sprite->setPosition(nPos.x,nPos.y);
}

void Piece::updatePos(sf::Vector2i bPos, int nPos)
{
	int x = nPos % 8;
	int y = nPos / 8;
	pos = sf::Vector2i(x,y);
	sprite->setPosition(bPos.x+pos.x*tSize+tSize/2,bPos.y+pos.y*tSize+tSize/2);
}

void Piece::render(sf::RenderWindow* window)
{
	window->draw(*sprite);
}

bool Piece::isInBounds(sf::Vector2i checked) const
{
	checked -= (sf::Vector2i)(sprite->getPosition()-sprite->getOrigin());
	return(checked.x > 0 && checked.y > 0 && checked.x < sprite->getLocalBounds().width&& checked.y < sprite->getLocalBounds().height);
}
