#pragma once

#include "IsoEngine.h"
#include <map>
#include <string>

class SpriteFactory
{
public:
	static void Cleanup();

	static SpritePtr GetSprite(size_t hash);
	static SpritePtr GetSprite(const std::string& texturePath);

	static SpritePtr GetSprite(size_t hash, sf::IntRect& rect);
	static SpritePtr GetSprite(const std::string& texturePath, sf::IntRect& rect);

	static size_t AddTexture(const std::string& texturePath);

	static sf::Vector2u GetTextureSize(size_t hash);

private:

	static void LoadTexture(size_t hash);

	static std::map<size_t, TexturePtr> SpriteTextures;
	static std::map<size_t, std::string> TexturePaths;
};


