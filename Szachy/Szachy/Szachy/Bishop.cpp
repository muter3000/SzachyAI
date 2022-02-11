#include "Bishop.h"

Bishop::Bishop(sf::Vector2i pos, bool white, sf::Vector2i boardPos)
	:Piece(pos)
{
	color = white;
	if (white)
	{
		static sf::Texture textureWBishop = []() {sf::Texture t; t.loadFromFile("Sprites/wBishop.png"); return t; }();
		this->pieceType = PieceType::wBishop;
		this->sprite = new sf::Sprite(textureWBishop);
		sf::FloatRect sBounds = sprite->getLocalBounds();
		this->sprite->setOrigin(sBounds.left + sBounds.width / 2, sBounds.top + sBounds.height / 2);
		this->sprite->setPosition(boardPos.x + pos.x * tSize + tSize / 2, boardPos.y + pos.y * tSize + tSize / 2);
		sprite->setScale(0.45f, 0.45f);
	}
	else
	{
		static sf::Texture textureBBishop = []() {sf::Texture t; t.loadFromFile("Sprites/bBishop.png"); return t; }();
		this->pieceType = PieceType::bBishop;
		this->sprite = new sf::Sprite(textureBBishop);
		sf::FloatRect sBounds = sprite->getLocalBounds();
		this->sprite->setOrigin(sBounds.left + sBounds.width / 2, sBounds.top + sBounds.height / 2);
		this->sprite->setPosition(boardPos.x + pos.x * tSize + tSize / 2, boardPos.y + pos.y * tSize + tSize / 2);
		sprite->setScale(0.45f, 0.45f);
	}
}
