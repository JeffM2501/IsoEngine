#include "ResourceManager.h"
#include <algorithm>

std::string GetResourcesDir()
{
#ifdef SFML_SYSTEM_IOS
	return "";
#else
	return "resources/";
#endif
}


std::string ResourceManager::ForceFilePath(const std::string& rootPath, const std::string& file)
{
	std::string fullPath = file;
	auto term = fullPath.find_last_of('/');
	if (term != std::string::npos)
		fullPath = fullPath.substr(term+1);

	return rootPath + "/" + fullPath;
}

std::string ResourceManager::GetNativeResourcePath(const std::string& file)
{
#ifdef _WINDOWS
	std::string realFile = file;
	std::replace(realFile.begin(), realFile.end(), '/', '\\'); // replace all 'x' to 'y'
	return realFile;
#else
	return file;
#endif
}

std::string ResourceManager::GetResourcePath(const std::string& file)
{
	return GetResourcesDir() + file;
}

std::string ResourceManager::GetRelativeResource(const std::string& rootFile, const std::string& relFile)
{
	std::string fullPath = rootFile;
	auto term = fullPath.find_last_of('/');
	if (term != std::string::npos)
		fullPath = fullPath.substr(0, term);

	return GetResourcePath(fullPath + "/" + relFile);
}