#pragma once

#include "IsoEngine.h"

class Tile
{
public:
	Tile(){};
	Tile(sf::Vector2f pos);

	void SetSprite(SpritePtr sprite, sf::Vector2u nominalSize);

	void Draw(RenderWindowPtr window, const sf::Vector2f &viewOffset);

	inline const sf::Vector2f& GetMapPostion() { return MapPostion; }

protected:
	sf::Vector2f MapPostion;
	sf::Vector2f DrawPostion;
	SpritePtr	Sprite;
};
