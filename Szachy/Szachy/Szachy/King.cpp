#include "King.h"

King::King(sf::Vector2i pos, bool white, sf::Vector2i boardPos)
	:Piece(pos)
{
	color = white;
	if (white)
	{
		static sf::Texture textureWKing = []() {sf::Texture t; t.loadFromFile("Sprites/wKing.png"); return t; }();
		this->pieceType = PieceType::wKing;
		this->sprite = new sf::Sprite(textureWKing);
		sf::FloatRect sBounds = sprite->getLocalBounds();
		this->sprite->setOrigin(sBounds.left + sBounds.width / 2, sBounds.top + sBounds.height / 2);
		this->sprite->setPosition(boardPos.x + pos.x * tSize + tSize / 2, boardPos.y + pos.y * tSize + tSize / 2);
		sprite->setScale(0.45f, 0.45f);
	}
	else
	{
		static sf::Texture textureBKing = []() {sf::Texture t; t.loadFromFile("Sprites/bKing.png"); return t; }();
		this->pieceType = PieceType::bKing;
		this->sprite = new sf::Sprite(textureBKing);
		sf::FloatRect sBounds = sprite->getLocalBounds();
		this->sprite->setOrigin(sBounds.left + sBounds.width / 2, sBounds.top + sBounds.height / 2);
		this->sprite->setPosition(boardPos.x + pos.x * tSize + tSize / 2, boardPos.y + pos.y * tSize + tSize / 2);
		sprite->setScale(0.45f, 0.45f);
	}
}
