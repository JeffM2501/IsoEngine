#pragma once

#include "IsoEngine.h"

class Tile
{
public:
	Tile(sf::Vector2f pos, SpritePtr base);

	void Draw(RenderWindowPtr window);

protected:
	sf::Vector2f MapPostion;
	SpritePtr	Sprite;
};