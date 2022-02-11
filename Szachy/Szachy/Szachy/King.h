#pragma once
#include "Piece.h"
class King :
    public Piece
{
public:
    King(sf::Vector2i pos, bool white, sf::Vector2i boardPos);
};

