#include "Tile.h"

Tile::Tile(sf::Vector2f pos) : MapPostion(pos)
{
	DrawPostion = MapPostion;
}

void Tile::SetSprite(SpritePtr sprite, sf::Vector2i nominalSize)
{
	Sprite = sprite;
	auto spriteSize = sprite->getTexture()->getSize();
	int offsetY = nominalSize.y - spriteSize.y;
	DrawPostion.x = MapPostion.x;
	DrawPostion.y = MapPostion.y + offsetY;
}

void Tile::Draw(RenderWindowPtr window, const sf::Vector2f &viewOffset)
{
	if (Sprite != nullptr)
	{
		Sprite->setPosition(DrawPostion + viewOffset);
		Sprite->setColor(Selected ? sf::Color::Magenta : sf::Color::White);
		window->draw(*Sprite);
	}	
}
