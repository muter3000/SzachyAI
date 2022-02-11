#pragma once
#include "Piece.h"
class Bishop :
    public Piece
{
public:
    Bishop(sf::Vector2i pos, bool white, sf::Vector2i boardPos);
};

