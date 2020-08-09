#include "IsoMap.h"
#include "ResourceManager.h"

bool IsoMap::LoadResources()
{
	TileTextures.push_back(SpriteFactory::LoadTexture("Tiles/iso_tile.png"));
	TileTextures.push_back(SpriteFactory::LoadTexture("Tiles/iso_tile2.png"));
	TileTextures.push_back(SpriteFactory::LoadTexture("Tiles/iso_tileGoal.png"));
	HighlightTexture = SpriteFactory::LoadTexture("Tiles/iso_tile_hl.png");
	return true;
}

sf::Vector2f IsoMap::MapGridToIso(float x, float y)
{
	float halfWidth = TileSize.x * 0.5f;
	float halfHeight = TileSize.y * 0.5f;
	float quarterHeight = TileSize.y * 0.25f;

	return sf::Vector2f(x * halfWidth - y * halfWidth - halfWidth, y * halfHeight + (x * halfHeight));
}

sf::Vector2u IsoMap::ScreenToMapGridIso(const sf::Vector2i& mousePos)
{
	sf::Vector2f worldSpacePoint((float)mousePos.x - ViewOffset.x, (float)mousePos.y - ViewOffset.y);

	float scale = 1.0f;

	sf::Vector2f tileSize = sf::Vector2f(TileSize.x * scale, TileSize.y * scale);

	int mouseXTile = int(worldSpacePoint.y / tileSize.y + worldSpacePoint.x / tileSize.x);
	int mouseYTile = int(-worldSpacePoint.x / tileSize.x + worldSpacePoint.y / tileSize.y);

	return ClampToMap(sf::Vector2i(mouseXTile, mouseYTile));
}

sf::Vector2u IsoMap::ClampToMap(sf::Vector2i pos)
{
	sf::Vector2u target;

	if (pos.x > (int)MapSize.x - 1)
		target.x = MapSize.x - 1;
	else
		target.x = pos.x;

	if (target.x < 0)
		target.x = 0;

	if (pos.y > (int)MapSize.y - 1)
		target.y = MapSize.y - 1;
	else
		target.y = pos.y;

	if (target.y < 0)
		target.y = 0;

	return target;
}

bool IsoMap::LoadMap()
{
	if (WindowPtr == nullptr || TileTextures.size() == 0)
		return false;

	TileSize = SpriteFactory::GetTextureSize(TileTextures[0]);
	auto windowSize = WindowPtr->getSize();

	float halfWidth = TileSize.x * 0.5f;
	float halfHeight = TileSize.y * 0.5f;

	MapSize.x = 20;
	MapSize.y = 20;

	for (float y = 0; y < MapSize.y; y++)
	{
		std::vector<Tile> row;
		for (float x = 0; x < MapSize.x; x++)
		{
			auto sprite = SpriteFactory::GetSprite(TileTextures[0]);
			row.push_back(Tile(MapGridToIso(x,y), sprite));
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
			t.Draw(WindowPtr, ViewOffset);
	}
		
	if (HighlightSprite != nullptr)
		WindowPtr->draw(*HighlightSprite);

	return true;
}

void IsoMap::SetMousePostion(const sf::Vector2i& pos)
{
	if (WindowPtr == nullptr)
		return;

	auto mapTile = ScreenToMapGridIso(pos);

	if (mapTile.x < 0 || mapTile.x >= MapSize.x || mapTile.y < 0 || mapTile.y >= MapSize.y)
	{
		if (HighlightSprite != nullptr)
			HighlightSprite = nullptr;
	}
	else
	{
		if (HighlightSprite == nullptr)
			HighlightSprite = SpriteFactory::GetSprite(HighlightTexture);
		
		HighlightSprite->setPosition(MapGridToIso(mapTile) + ViewOffset);
	}
}

void IsoMap::MoveView(sf::Vector2f offset)
{
	ViewOffset = offset;
}

void IsoMap::CenterMap()
{
	if (WindowPtr == nullptr)
		return;

	ViewOffset = MapGridToIso(MapSize.x * 0.5f, MapSize.y * 0.5f);
	ViewOffset.x += WindowPtr->getSize().x * 0.5f;
}

void IsoMap::Cleanup()
{
	Tiles.clear();
	TileTextures.clear();
	WindowPtr = nullptr;
}
