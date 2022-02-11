#pragma once
#include "Piece.h"
class Night :
    public Piece
{
public:
    Night(sf::Vector2i pos, bool white, sf::Vector2i boardPos);
};

