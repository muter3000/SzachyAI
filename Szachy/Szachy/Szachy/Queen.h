#pragma once
#include "Piece.h"
class Queen :
    public Piece
{
public:
    Queen(sf::Vector2i pos, bool white, sf::Vector2i boardPos);
};

