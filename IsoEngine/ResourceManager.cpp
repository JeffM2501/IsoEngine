#include "ResourceManager.h"

std::string GetResourcesDir()
{
#ifdef SFML_SYSTEM_IOS
	return "";
#else
	return "resources/";
#endif
}

std::string ResourceManager::GetResourcePath(const std::string& file)
{
	return GetResourcesDir() + file;
}