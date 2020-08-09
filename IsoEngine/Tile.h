#pragma once

#include "IsoEngine.h"

class Tile
{
public:
	Tile(){};
	Tile(sf::Vector2f pos, SpritePtr base);

	void SetSprite(SpritePtr sprite);

	void Draw(RenderWindowPtr window, const sf::Vector2f &viewOffset);

	inline const sf::Vector2f& GetMapPostion() { return MapPostion; }

protected:
	sf::Vector2f MapPostion;
	SpritePtr	Sprite;
};
