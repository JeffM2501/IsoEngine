#pragma once
#include "IsoEngine.h"
#include "Tile.h"
#include "SpriteFactory.h"

class IsoMap
{
protected:
	RenderWindowPtr WindowPtr = nullptr;
	sf::Vector2u TileSize;
	sf::Vector2u MapSize;

	sf::Vector2f MapGridToIso(float x, float y);
	inline sf::Vector2f MapGridToIso(sf::Vector2u& pos) { return MapGridToIso((float)pos.x, (float)pos.y); }

	sf::Vector2u ScreenToMapGridIso(const sf::Vector2i& pos);

	sf::Vector2f ViewOffset;
public:

	IsoMap(RenderWindowPtr winPtr) : WindowPtr(winPtr){}

	bool LoadResources();
	bool LoadMap();
	bool Draw();

	void SetMousePostion(const sf::Vector2i &pos);
	void MoveView(sf::Vector2f offset);
	void CenterMap();
	void Cleanup();

	sf::Vector2u ClampToMap(sf::Vector2i pos);

private:
	std::vector<size_t> TileTextures;
	std::vector<std::vector<Tile>> Tiles;

	size_t HighlightTexture = 0;
	SpritePtr HighlightSprite;
};

