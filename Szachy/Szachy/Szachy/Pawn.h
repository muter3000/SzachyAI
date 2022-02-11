#pragma once
#include "Piece.h"
class Pawn :
    public Piece
{
public:
	Pawn(sf::Vector2i pos, bool white, sf::Vector2i boardPos);
	bool moved;
};

