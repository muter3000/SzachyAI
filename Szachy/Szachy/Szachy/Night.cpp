#include "Night.h"

Night::Night(sf::Vector2i pos, bool white, sf::Vector2i boardPos)
	:Piece(pos)
{
	color = white;
	if (white)
	{
		static sf::Texture textureWNight = []() {sf::Texture t; t.loadFromFile("Sprites/wNight.png"); return t; }();
		this->pieceType = PieceType::wNight;
		this->sprite = new sf::Sprite(textureWNight);
		sf::FloatRect sBounds = sprite->getLocalBounds();
		this->sprite->setOrigin(sBounds.left + sBounds.width / 2, sBounds.top + sBounds.height / 2);
		this->sprite->setPosition(boardPos.x + pos.x * tSize + tSize / 2, boardPos.y + pos.y * tSize + tSize / 2);
		sprite->setScale(0.45f, 0.45f);
	}
	else
	{
		static sf::Texture textureBNight = []() {sf::Texture t; t.loadFromFile("Sprites/bNight.png"); return t; }();
		this->pieceType = PieceType::bNight;
		this->sprite = new sf::Sprite(textureBNight);
		sf::FloatRect sBounds = sprite->getLocalBounds();
		this->sprite->setOrigin(sBounds.left + sBounds.width / 2, sBounds.top + sBounds.height / 2);
		this->sprite->setPosition(boardPos.x + pos.x * tSize + tSize / 2, boardPos.y + pos.y * tSize + tSize / 2);
		sprite->setScale(0.45f, 0.45f);
	}
}
