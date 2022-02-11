#pragma once
#include <SFML/Graphics.hpp>
#include "Variables.h"
#include "Move.h"

class Piece
{
protected:
	PieceType pieceType;
	bool color;
	sf::Vector2i pos;
	sf::Sprite* sprite;
	sf::Texture tex;
	std::pair<bool,int> pin;
public:
	Piece(sf::Vector2i pos);
	virtual ~Piece();
	sf::Vector2i getPos();
	int getPosInt();
	void updatePosLive(sf::Vector2i nPos);
	void updatePos(sf::Vector2i bPos,int nPos);
	void render(sf::RenderWindow* window);
	bool isInBounds(sf::Vector2i checked) const;

	PieceType getPieceType() { return pieceType; }
	bool getPieceColor() { return color; }
	void setPinned(int direction) { pin = std::pair<bool,int>(true,direction); }
	void unPin() { pin.first = false; }
	std::pair<bool,int> getPinned() { return pin; }
};

