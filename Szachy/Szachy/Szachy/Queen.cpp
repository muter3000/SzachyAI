#include "Queen.h"

Queen::Queen(sf::Vector2i pos, bool white, sf::Vector2i boardPos)
	:Piece(pos)
{
	color = white;
	if (white)
	{
		static sf::Texture textureWQueen = []() {sf::Texture t; t.loadFromFile("Sprites/wQueen.png"); return t; }();
		this->pieceType = PieceType::wQueen;
		this->sprite = new sf::Sprite(textureWQueen);
		sf::FloatRect sBounds = sprite->getLocalBounds();
		this->sprite->setOrigin(sBounds.left + sBounds.width / 2, sBounds.top + sBounds.height / 2);
		this->sprite->setPosition(boardPos.x + pos.x * tSize + tSize / 2, boardPos.y + pos.y * tSize + tSize / 2);
		sprite->setScale(0.45f, 0.45f);
	}
	else
	{
		static sf::Texture textureBQueen = []() {sf::Texture t; t.loadFromFile("Sprites/bQueen.png"); return t; }();
		this->pieceType = PieceType::bQueen;
		this->sprite = new sf::Sprite(textureBQueen);
		sf::FloatRect sBounds = sprite->getLocalBounds();
		this->sprite->setOrigin(sBounds.left + sBounds.width / 2, sBounds.top + sBounds.height / 2);
		this->sprite->setPosition(boardPos.x + pos.x * tSize + tSize / 2, boardPos.y + pos.y * tSize + tSize / 2);
		sprite->setScale(0.45f, 0.45f);
	}
}
