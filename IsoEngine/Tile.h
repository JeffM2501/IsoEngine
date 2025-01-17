#pragma once

#include "IsoEngine.h"
#include "Viewport.h"

class Tile
{
public:
	Tile(){};
	Tile(sf::Vector2f pos);

	void SetSprite(SpritePtr sprite, sf::Vector2i nominalSize);

	void Draw(Viewport::Ptr view);

	inline sf::Vector2f GetMapPostion() { return MapPostion; }
	inline const sf::Vector2f& GetDrawPostion() { return DrawPostion; }
	
	inline bool IsSelected() { return Selected; }
	inline void ToggleSelection() { Selected = !Selected; }

	inline bool IsEmpty() { return Sprite == nullptr; }

protected:
	sf::Vector2f MapPostion;
	sf::Vector2f DrawPostion;
	SpritePtr	Sprite;

	bool Selected = false;
};
