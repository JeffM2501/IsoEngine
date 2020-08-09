#include "Tile.h"

Tile::Tile(sf::Vector2f pos, SpritePtr base) : MapPostion(pos), Sprite(base)
{
	Sprite->setPosition(MapPostion);
}

void Tile::Draw(RenderWindowPtr window)
{
	if (Sprite != nullptr)
		window->draw(*Sprite);
}
