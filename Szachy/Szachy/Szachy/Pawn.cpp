#include "Pawn.h"

Pawn::Pawn(sf::Vector2i pos, bool white, sf::Vector2i boardPos)
	:Piece(pos), moved(false)
{
	color = white;
	if (white)
	{
		static sf::Texture textureWPawn = []() {sf::Texture t; t.loadFromFile("Sprites/wPawn.png"); return t; }();
		this->pieceType = PieceType::wPawn;
		this->sprite = new sf::Sprite(textureWPawn);
		sf::FloatRect sBounds = sprite->getLocalBounds();
		this->sprite->setOrigin(sBounds.left + sBounds.width / 2, sBounds.top + sBounds.height / 2);
		this->sprite->setPosition(boardPos.x + pos.x * tSize + tSize / 2, boardPos.y + pos.y * tSize + tSize / 2);
		sprite->setScale(0.45f, 0.45f);
	}
	else
	{
		static sf::Texture textureBPawn = []() {sf::Texture t; t.loadFromFile("Sprites/bPawn.png"); return t; }();
		this->pieceType = PieceType::bPawn;
		this->sprite = new sf::Sprite(textureBPawn);
		sf::FloatRect sBounds = sprite->getLocalBounds();
		this->sprite->setOrigin(sBounds.left + sBounds.width / 2, sBounds.top + sBounds.height / 2);
		this->sprite->setPosition(boardPos.x + pos.x * tSize + tSize / 2, boardPos.y + pos.y * tSize + tSize / 2);
		sprite->setScale(0.45f, 0.45f);
	}
}