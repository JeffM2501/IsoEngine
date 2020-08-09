#pragma once
#include "IsoEngine.h"

class ResourceManager
{
public:
	static std::string ForceFilePath(const std::string& rootPath, const std::string& file);
	static std::string GetNativeResourcePath(const std::string& file);
	static std::string GetResourcePath(const std::string& file);
	static std::string GetRelativeResource(const std::string& rootFile, const std::string& relFile);

private:
};