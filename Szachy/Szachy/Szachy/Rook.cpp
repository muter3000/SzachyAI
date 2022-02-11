#include "Rook.h"

Rook::Rook(sf::Vector2i pos, bool white, sf::Vector2i boardPos)
	:Piece(pos)
{
	color = white;
	if (white)
	{
		static sf::Texture textureWRook = []() {sf::Texture t; t.loadFromFile("Sprites/wRook.png"); return t; }();
		this->pieceType = PieceType::wRook;
		this->sprite = new sf::Sprite(textureWRook);
		sf::FloatRect sBounds = sprite->getLocalBounds();
		this->sprite->setOrigin(sBounds.left + sBounds.width / 2, sBounds.top + sBounds.height / 2);
		this->sprite->setPosition(boardPos.x + pos.x * tSize + tSize / 2, boardPos.y + pos.y * tSize + tSize / 2);
		sprite->setScale(0.45f, 0.45f);
	}
	else
	{
		static sf::Texture textureBRook = []() {sf::Texture t; t.loadFromFile("Sprites/bRook.png"); return t; }();
		this->pieceType = PieceType::bRook;
		this->sprite = new sf::Sprite(textureBRook);
		sf::FloatRect sBounds = sprite->getLocalBounds();
		this->sprite->setOrigin(sBounds.left + sBounds.width / 2, sBounds.top + sBounds.height / 2);
		this->sprite->setPosition(boardPos.x + pos.x * tSize + tSize / 2, boardPos.y + pos.y * tSize + tSize / 2);
		sprite->setScale(0.45f, 0.45f);
	}
}
