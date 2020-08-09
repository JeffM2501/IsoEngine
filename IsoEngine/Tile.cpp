#include "Tile.h"

Tile::Tile(sf::Vector2f pos, SpritePtr base) : MapPostion(pos), Sprite(base)
{
	SetSprite(Sprite);
}

void Tile::SetSprite(SpritePtr sprite)
{
	Sprite = sprite;
}

void Tile::Draw(RenderWindowPtr window, const sf::Vector2f &viewOffset)
{
	if (Sprite != nullptr)
	{
		Sprite->setPosition(MapPostion + viewOffset);
		window->draw(*Sprite);
	}	
}
