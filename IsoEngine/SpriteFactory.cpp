#include "SpriteFactory.h"
#include "ResourceManager.h"

#include <exception>

std::map<size_t, TexturePtr> SpriteFactory::SpriteTextures;
std::map<size_t, std::string> SpriteFactory::TexturePaths;

void SpriteFactory::Cleanup()
{
	SpriteTextures.clear();
}

SpritePtr SpriteFactory::GetSprite(size_t hash)
{
	auto texItr = SpriteTextures.find(hash);
	if (texItr == SpriteTextures.end())
	{
		LoadTexture(hash);
		texItr = SpriteTextures.find(hash);
		if (texItr == SpriteTextures.end())
			throw std::runtime_error("Unable to locate texture hash " + hash);
	}
		
	return std::make_shared<sf::Sprite>(*(texItr->second));
}

void SpriteFactory::LoadTexture(size_t hash)
{
	auto pathItr = TexturePaths.find(hash);
	if (pathItr == TexturePaths.end())
		throw std::runtime_error("Unable to locate texture path for has " + hash);

	std::string texturePath = TexturePaths[hash];

	TexturePtr tex = std::make_shared<sf::Texture>();
	if (!tex->loadFromFile(texturePath))
		throw std::runtime_error("Unable to locate texture path " + texturePath);

	SpriteTextures[hash] = tex;
}

size_t SpriteFactory::LoadTexture(const std::string& texturePath)
{
	size_t hash = std::hash<std::string>()(texturePath);

	auto texItr = SpriteTextures.find(hash);
	if (texItr != SpriteTextures.end())
		return hash;

	if (TexturePaths.find(hash) == TexturePaths.end())
	{
		std::string realPath = ResourceManager::GetResourcePath(texturePath);
		TexturePaths[hash] = realPath;
		LoadTexture(hash);
	}
	
	return hash;
}

sf::Vector2u SpriteFactory::GetTextureSize(size_t hash)
{
	auto texItr = SpriteTextures.find(hash);
	if (texItr == SpriteTextures.end())
	{
		LoadTexture(hash);
		texItr = SpriteTextures.find(hash);
		if (texItr == SpriteTextures.end())
			throw std::runtime_error("Unable to locate texture hash " + hash);
	}
		
	return texItr->second->getSize();
}

SpritePtr SpriteFactory::GetSprite(const std::string& texturePath)
{
	return GetSprite(LoadTexture(texturePath));
}
