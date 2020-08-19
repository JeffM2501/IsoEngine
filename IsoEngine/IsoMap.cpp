#include "IsoMap.h"
#include "ResourceManager.h"
#include "TiledMapReader.h"

bool IsoMap::LoadResources()
{
	if (MapType == MapTypes::Orthographic)
		HighlightTexture = SpriteFactory::LoadTexture("Tiles/Tile_HL.png");
	else
		HighlightTexture = SpriteFactory::LoadTexture("Highlight.png");
	return true;
}

sf::Vector2f IsoMap::GetTileDisplayLocation(float x, float y)
{
	if (MapType == MapTypes::Orthographic)
		return sf::Vector2f(x * TileSize.x, y * TileSize.y);

	float halfWidth = TileSize.x * 0.5f;
	float halfHeight = TileSize.y * 0.5f;
	float quarterHeight = TileSize.y * 0.25f;

	return sf::Vector2f(x * halfWidth - y * halfWidth - halfWidth, y * halfHeight + (x * halfHeight));
}

sf::Vector2i IsoMap::WorldToMap(const sf::Vector2f& mousePos)
{
	if (MapType == MapTypes::Orthographic)
		return sf::Vector2i((int)(mousePos.y) / TileSize.y, (int)(mousePos.x) / TileSize.x);

	sf::Vector2f worldSpacePoint(mousePos.x, mousePos.y);

	float scale = 1.0f;

	sf::Vector2f tileSize = sf::Vector2f(TileSize.x * scale, TileSize.y * scale);

	int mouseYTile = int(worldSpacePoint.y / tileSize.y + worldSpacePoint.x / tileSize.x);
	int mouseXTile = int(-worldSpacePoint.x / tileSize.x + worldSpacePoint.y / tileSize.y);

	return ClampToMap(sf::Vector2i(mouseXTile, mouseYTile));
}

bool IsoMap::PointOnMap(sf::Vector2i pos)
{
	return (pos.x < static_cast<int>(MapSize.x) && pos.x >= 0 && pos.y < static_cast<int>(MapSize.y) && pos.y >= 0);
}

sf::Vector2i IsoMap::ClampToMap(sf::Vector2i pos)
{
	sf::Vector2i target;

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

Tile* IsoMap::GetTile(sf::Vector2i pos, int layer)
{
	if (!PointOnMap(pos))
		return nullptr;

	return &(Layers[layer][pos.y][pos.x]);
}

size_t IsoMap::AddTileSetTile(int textureID, const std::string& tile)
{
	TileTextures[textureID] = SpriteFactory::LoadTexture(tile);
	return TileTextures[textureID];
}

void IsoMap::SetLayerTile(int layerID, sf::Vector2i pos, int textureID)
{
	auto layer = Layers.find(layerID);
	if (layer == Layers.end())
		SetupLayer(layerID);

	layer = Layers.find(layerID);

	auto coord = ClampToMap(pos);

	SpritePtr sprite = nullptr;
	if (TileTextures.find(textureID) != TileTextures.end())
		sprite = SpriteFactory::GetSprite(TileTextures[textureID]);

	layer->second[coord.x][coord.y].SetSprite(sprite, TileSize);
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
			col.push_back(Tile(GetTileDisplayLocation(x, y)));
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
	if (ViewPtr == nullptr)
		return false;

 	TiledMapReader reader;
	if (!reader.Read(mapFile, *this))
		return false;

	ViewPtr->SetViewRectTolerance(TileSize.x, TileSize.y);
	return true;
}

bool IsoMap::Draw()
{
	if (ViewPtr == nullptr)
		return false;

	bool first = true;

	for (auto layer : Layers)
	{
		for (auto cols : layer.second)
		{
			for (auto tile : cols)
			{
				if (ViewPtr->TileInView(tile.GetDrawPostion()))
					tile.Draw(ViewPtr);
			}		
		}

		if (first)
		{
			if (HighlightSprite != nullptr && DoHighlights)
			{
				auto tile = GetTile(TileUnderCursor, layer.first);
				if (tile != nullptr)
				{
					// draw the highlight at the position of the base layer tile, so it appears on the "surface" of the isometric tile not the floor
					HighlightSprite->setPosition(tile->GetDrawPostion());
					ViewPtr->Window.draw(*HighlightSprite);
				}
			}
				
		}
		first = false;
	}

	return true;
}

void IsoMap::SetHighlight(bool mode)
{
	DoHighlights = mode;
}

sf::Vector2i IsoMap::GetTileUnderCursor()
{
	return TileUnderCursor;
}

void IsoMap::SelectTile(sf::Vector2i tilePos)
{
	auto tile = GetTile(tilePos, FirstLayer);
	if (tile != nullptr)
		tile->ToggleSelection();
}

void IsoMap::SetMousePostion(const sf::Vector2i& pos)
{
	if (ViewPtr == nullptr)
		return;

	TileUnderCursor = WorldToMap(ViewPtr->Window.mapPixelToCoords(pos));

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

			HighlightSprite->setPosition(GetTileDisplayLocation(TileUnderCursor));
		}
	}
}

void IsoMap::SetViewpoint(sf::Vector2f offset)
{
	ViewPtr->View.setCenter(offset);
	ViewPtr->SetView();
}

void IsoMap::MoveViewpoint(float x, float y)
{
	ViewPtr->View.move(x, y);
	ViewPtr->SetView();
}

void IsoMap::CenterMap()
{
	if (ViewPtr == nullptr)
		return;

	SetViewpoint(GetTileDisplayLocation(MapSize.x * 0.5f, MapSize.y * 0.5f) * -1.0f);
}

void IsoMap::Cleanup()
{
	ClearMap();
	TileTextures.clear();
	ViewPtr = nullptr;
}
