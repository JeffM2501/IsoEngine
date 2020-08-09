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

public:

	IsoMap(RenderWindowPtr winPtr) : WindowPtr(winPtr){}

	bool LoadResources();
	bool LoadMap();
	bool Draw();

	void SetMousePostion(const sf::Vector2i &pos);

	void Cleanup();

private:
	std::vector<size_t> TileTextures;
	std::vector<std::vector<Tile>> Tiles;

	size_t HighlightTexture = 0;
	SpritePtr HighlightSprite;
};

