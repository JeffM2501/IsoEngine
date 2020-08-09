#pragma once
#include "IsoEngine.h"
#include "Tile.h"
#include "SpriteFactory.h"
#include <optional>

class IsoMap
{
protected:
	RenderWindowPtr WindowPtr = nullptr;
	sf::Vector2u TileSize;
	sf::Vector2u MapSize;

	sf::Vector2f MapGridToIso(float x, float y);
	inline sf::Vector2f MapGridToIso(sf::Vector2u& pos)
	{ 
		return MapGridToIso((float)pos.x, (float)pos.y); 
	}

	sf::Vector2u ScreenToMapGridIso(const sf::Vector2i& pos);

	sf::Vector2f ViewOffset;

	void SetupLayer(int layerID);
public:

	IsoMap(RenderWindowPtr winPtr) : WindowPtr(winPtr){}

	bool LoadResources();
	bool LoadMap(const std::string& mapFile);
	bool Draw();

	void SetMousePostion(const sf::Vector2i &pos);
	void SetViewpoint(sf::Vector2f offset);
	void MoveViewpoint(float x, float y);
	void CenterMap();
	void Cleanup();

	sf::Vector2u ClampToMap(sf::Vector2i pos);
	bool PointOnMap(sf::Vector2i pos);

	std::optional<Tile> GetTile(sf::Vector2i pos, int layer = 0);

	inline std::optional<Tile> GetTile(int x, int y)
	{ 
		return GetTile(sf::Vector2i(x, y));
	}

	inline void SetMapSize(int x, int y)
	{
		MapSize = sf::Vector2u(x, y);
	}

	inline void SetTileSize(int x, int y)
	{
		TileSize = sf::Vector2u(x, y);
	}

	size_t AddTileSetTile(const std::string& tile);

	void SetLayerTile(int layer, sf::Vector2i pos, int textureIndex);

	void ClearLayer(int layer);
	void ClearMap();

	void SetHighlight(bool mode);
	sf::Vector2u GetTileUnderCursor();

private:
	typedef std::vector<Tile> TileColumn;
	typedef std::vector<TileColumn> TileLayer;

	std::vector<size_t> TileTextures;
	std::map<int, TileLayer> Layers;

	bool DoHighlights = true;
	sf::Vector2u TileUnderCursor;
	size_t HighlightTexture = 0;
	SpritePtr HighlightSprite;
};

