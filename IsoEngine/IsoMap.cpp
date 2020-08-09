#include "IsoMap.h"
#include "ResourceManager.h"
#include "TiledMapReader.h"

bool IsoMap::LoadResources()
{
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

bool IsoMap::PointOnMap(sf::Vector2i pos)
{
	return (pos.x < static_cast<int>(MapSize.x) && pos.x >= 0 && pos.y < static_cast<int>(MapSize.y) && pos.y >= 0);
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

std::optional<Tile> IsoMap::GetTile(sf::Vector2i pos, int layer)
{
	if (!PointOnMap(pos))
		return std::nullopt;

	return Layers[layer][pos.y][pos.x];
}

size_t IsoMap::AddTileSetTile(const std::string& tile)
{
	size_t id = TileTextures.size();
	TileTextures.push_back(SpriteFactory::LoadTexture(tile));
	return id;
}

void IsoMap::SetLayerTile(int layerID, sf::Vector2i pos, int textureIndex)
{
	auto layer = Layers.find(layerID);
	if (layer == Layers.end())
		SetupLayer(layerID);

	layer = Layers.find(layerID);

	auto coord = ClampToMap(pos);

	if (textureIndex < 0 || textureIndex >= TileTextures.size())
		return;

	layer->second[coord.x][coord.y].SetSprite(SpriteFactory::GetSprite(TileTextures[textureIndex]));
}

void IsoMap::SetupLayer(int layerID)
{
	// assignment will clear out any existing layer
	Layers[layerID] = TileLayer();

	TileLayer& layerRef = Layers[layerID];

	for (float x = 0; x < MapSize.x; x++)
	{
		TileColumn col;
		col.reserve(MapSize.y);
		for (float y = 0; y < MapSize.y; y++)
		{
			col.push_back(Tile(MapGridToIso(x, y), nullptr));
		}
		layerRef.push_back(col);
	}
}

void IsoMap::ClearLayer(int layerID)
{
	auto layer = Layers.find(layerID);
	if (layer != Layers.end())
		Layers.erase(layer);
}

void IsoMap::ClearMap()
{
	Layers.clear();
}

bool IsoMap::LoadMap(const std::string& mapFile)
{
	if (WindowPtr == nullptr)
		return false;

 	TiledMapReader reader;
	return reader.Read(mapFile, *this);
}

bool IsoMap::Draw()
{
	if (WindowPtr == nullptr)
		return false;

	sf::Rect<float> mapViewRegon;

	mapViewRegon.width = static_cast<float>(WindowPtr->getSize().x) + (TileSize.x * 2);
	mapViewRegon.height = static_cast<float>(WindowPtr->getSize().y) + (TileSize.y * 2);

	mapViewRegon.left = -ViewOffset.x - (TileSize.x * 2);
	mapViewRegon.top = -ViewOffset.y - (TileSize.y * 2);

	for (auto layer : Layers)
	{
		for (auto cols : layer.second)
		{
			for (auto tile : cols)
			{
				if (mapViewRegon.contains(tile.GetMapPostion()))
					tile.Draw(WindowPtr, ViewOffset);
			}		
		}
	}
		
	if (HighlightSprite != nullptr)
		WindowPtr->draw(*HighlightSprite);

	return true;
}

void IsoMap::SetHighlight(bool mode)
{
	DoHighlights = mode;
}

sf::Vector2u IsoMap::GetTileUnderCursor()
{
	return TileUnderCursor;
}

void IsoMap::SetMousePostion(const sf::Vector2i& pos)
{
	if (WindowPtr == nullptr)
		return;

	TileUnderCursor = ScreenToMapGridIso(pos);

	if (!DoHighlights && HighlightSprite != nullptr)
		HighlightSprite = nullptr;
	else
	{
		if (TileUnderCursor.x < 0 || TileUnderCursor.x >= MapSize.x || TileUnderCursor.y < 0 || TileUnderCursor.y >= MapSize.y)
		{
			if (HighlightSprite != nullptr)
				HighlightSprite = nullptr;
		}
		else
		{
			if (HighlightSprite == nullptr)
				HighlightSprite = SpriteFactory::GetSprite(HighlightTexture);

			HighlightSprite->setPosition(MapGridToIso(TileUnderCursor) + ViewOffset);
		}
	}
}

void IsoMap::SetViewpoint(sf::Vector2f offset)
{
	ViewOffset = offset;
}

void IsoMap::MoveViewpoint(float x, float y)
{
	ViewOffset.x += x;
	ViewOffset.y += y;
}

void IsoMap::CenterMap()
{
	if (WindowPtr == nullptr)
		return;

	ViewOffset = MapGridToIso(MapSize.x * 0.5f, MapSize.y * 0.5f) * -1.0f;
	ViewOffset.x += WindowPtr->getSize().x * 0.5f;
	ViewOffset.y += WindowPtr->getSize().y * 0.5f;
}

void IsoMap::Cleanup()
{
	ClearMap();
	TileTextures.clear();
	WindowPtr = nullptr;
}
