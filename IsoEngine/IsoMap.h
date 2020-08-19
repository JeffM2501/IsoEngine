#pragma once
#include "IsoEngine.h"
#include "Tile.h"
#include "SpriteFactory.h"
#include "Viewport.h"
#include <optional>

enum class MapTypes
{
	Orthographic,
	Isometric,
};

class IsoMap
{
protected:
	Viewport::Ptr ViewPtr = nullptr;
	sf::Vector2i TileSize;
	sf::Vector2i MapSize;

	sf::Vector2f GetTileDisplayLocation(float x, float y);
	inline sf::Vector2f GetTileDisplayLocation(sf::Vector2i& pos)
	{ 
		return GetTileDisplayLocation((float)pos.x, (float)pos.y); 
	}

	sf::Vector2i WorldToMap(const sf::Vector2f& pos);

	void SetupLayer(int layerID);

	MapTypes MapType = MapTypes::Isometric;

public:

	IsoMap(Viewport::Ptr viewPtr) : ViewPtr(viewPtr){}

	bool LoadResources();
	bool LoadMap(const std::string& mapFile);
	bool Draw();

	void SetMousePostion(const sf::Vector2i &pos);
	void SetViewpoint(sf::Vector2f offset);
	void MoveViewpoint(float x, float y);
	void CenterMap();
	void Cleanup();

	sf::Vector2i ClampToMap(sf::Vector2i pos);
	bool PointOnMap(sf::Vector2i pos);

	Tile* GetTile(sf::Vector2i pos, int layer);

	inline Tile* GetTile(int x, int y, int layer)
	{ 
		return GetTile(sf::Vector2i(x, y), layer);
	}

	inline void SetMapSize(int x, int y, MapTypes mapType)
	{
		MapSize = sf::Vector2i(x, y);
		MapType = mapType;
	}

	inline void SetTileSize(int x, int y)
	{
		TileSize = sf::Vector2i(x, y);
	}

	size_t AddTileSetTile(int id, const std::string& tile);

	void SetLayerTile(int layer, sf::Vector2i pos, int textureID);

	void ClearLayer(int layer);
	void ClearMap();

	void SetHighlight(bool mode);
	sf::Vector2i GetTileUnderCursor();

	void SelectTile(sf::Vector2i tile);

	int FirstLayer = -1;

private:
	typedef std::vector<Tile> TileColumn;
	typedef std::vector<TileColumn> TileLayer;

	std::map<int, size_t> TileTextures;
	std::map<int, TileLayer> Layers;

	bool DoHighlights = true;
	sf::Vector2i TileUnderCursor;
	size_t HighlightTexture = 0;
	SpritePtr HighlightSprite;
};

