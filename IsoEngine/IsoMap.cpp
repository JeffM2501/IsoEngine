#include "IsoMap.h"
#include "ResourceManager.h"

bool IsoMap::LoadResources()
{
	TileTextures.push_back(SpriteFactory::LoadTexture("Tiles/tile.png"));
	HighlightTexture = SpriteFactory::LoadTexture("Tiles/Tile_HL.png");
	return true;
}

bool IsoMap::LoadMap()
{
	if (WindowPtr == nullptr || TileTextures.size() == 0)
		return false;

	TileSize = SpriteFactory::GetTextureSize(TileTextures[0]);
	auto windowSize = WindowPtr->getSize();

	MapSize.x = (windowSize.x / TileSize.x) + 1;
	MapSize.y = (windowSize.y / TileSize.y) + 1;

	for (float y = 0; y < MapSize.y; y++)
	{
		std::vector<Tile> row;
		for (float x = 0; x < MapSize.x; x++)
		{
			auto sprite = SpriteFactory::GetSprite(TileTextures[0]);
			row.push_back(Tile(sf::Vector2f(x * TileSize.x, y * TileSize.y), sprite));
		}
			
		Tiles.push_back(row);
	}

	return true;
}

bool IsoMap::Draw()
{
	if (WindowPtr == nullptr)
		return false;
	for (auto r : Tiles)
	{
		for (auto t : r)
			t.Draw(WindowPtr);
	}
		
	if (HighlightSprite != nullptr)
		WindowPtr->draw(*HighlightSprite);

	return true;
}

void IsoMap::SetMousePostion(const sf::Vector2i& pos)
{
	if (WindowPtr == nullptr)
		return;

	float xTile = (float)(pos.x / TileSize.x);
	float yTile = (float)(pos.y / TileSize.y);

	if (xTile < 0 || xTile >= MapSize.x || yTile < 0 || yTile >= MapSize.y)
	{
		if (HighlightSprite != nullptr)
			HighlightSprite = nullptr;
	}
	else
	{
		if (HighlightSprite == nullptr)
			HighlightSprite = SpriteFactory::GetSprite(HighlightTexture);
		
		HighlightSprite->setPosition(sf::Vector2f(xTile * TileSize.x, yTile * TileSize.y));
	}
}

void IsoMap::Cleanup()
{
	Tiles.clear();
	TileTextures.clear();
	WindowPtr = nullptr;
}
