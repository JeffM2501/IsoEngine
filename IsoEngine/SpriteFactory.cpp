#include "SpriteFactory.h"
#include "ResourceManager.h"

#include <exception>

std::map<size_t, TexturePtr> SpriteFactory::SpriteTextures;

void SpriteFactory::Cleanup()
{
	SpriteTextures.clear();
}

SpritePtr SpriteFactory::GetSprite(size_t hash)
{
	auto texItr = SpriteTextures.find(hash);
	if (texItr == SpriteTextures.end())
		throw std::runtime_error("Unable to locate texture hash " + hash);

	return std::make_shared<sf::Sprite>(*(texItr->second));
}

size_t SpriteFactory::LoadTexture(const std::string& texturePath)
{
	size_t hash = std::hash<std::string>()(texturePath);

	auto texItr = SpriteTextures.find(hash);
	if (texItr != SpriteTextures.end())
		return hash;

	TexturePtr tex = std::make_shared<sf::Texture>();
	if (!tex->loadFromFile(ResourceManager::GetResourcePath(texturePath)))
		throw std::runtime_error("Unable to locate texture path " + texturePath);

	SpriteTextures[hash] = tex;

	return hash;
}

sf::Vector2u SpriteFactory::GetTextureSize(size_t hash)
{
	auto texItr = SpriteTextures.find(hash);
	if (texItr == SpriteTextures.end())
		
		throw std::runtime_error("Unable to locate texture hash " + hash);
	return texItr->second->getSize();
}

SpritePtr SpriteFactory::GetSprite(const std::string& texturePath)
{
	return GetSprite(LoadTexture(texturePath));
}
