#include "Tile.h"

Tile::Tile(sf::Vector2f pos) : MapPostion(pos)
{
	DrawPostion = MapPostion;
}

void Tile::SetSprite(SpritePtr sprite, sf::Vector2i nominalSize)
{
	Sprite = sprite;
	if (sprite != nullptr)
	{
		auto spriteSize = sprite->getTexture()->getSize();
		int offsetY = nominalSize.y - spriteSize.y;
		DrawPostion.x = MapPostion.x;
		DrawPostion.y = MapPostion.y + offsetY;
	}
	else
		DrawPostion = MapPostion;

	if (Sprite != nullptr)
		Sprite->setPosition(DrawPostion);
}

void Tile::Draw(Viewport::Ptr view)
{
	if (Sprite != nullptr)
	{

		Sprite->setColor(Selected ? sf::Color::Magenta : sf::Color::White);
		view->Window.draw(*Sprite);
	}	
}
