#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

#include <memory>
#include <string>
#include <vector>

#ifdef _WINDOWS	
#include "resource.h"
#include "framework.h"
#endif

typedef std::shared_ptr<sf::Texture> TexturePtr;
typedef std::shared_ptr<sf::Sprite> SpritePtr;


