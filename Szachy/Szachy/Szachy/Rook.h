#pragma once
#include "Piece.h"
class Rook :
    public Piece
{
public:
    Rook(sf::Vector2i pos, bool white, sf::Vector2i boardPos);
};

