#pragma once
#include <SFML/Graphics.hpp>
class BoxBase
{
	protected:
		sf::Vector2f pos;
		sf::Font font;
		sf::RectangleShape box;
	public:
		BoxBase(sf::Vector2f _pos, sf::Vector2f _size);
		~BoxBase();
		void setPos(sf::Vector2f nPos);
		sf::Vector2f getPos();
		virtual void draw(sf::RenderWindow* window) = 0;
		virtual bool isInBounds(sf::Vector2i checked) const = 0;
};

