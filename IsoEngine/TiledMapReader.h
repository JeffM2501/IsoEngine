#pragma once
#include "IsoEngine.h"
#include "IsoMap.h"
#include "PUGIXML/pugixml.hpp"

class TiledMapReader
{
public:
	static bool Read(const std::string& mapFilename, IsoMap& map);

private:
	static bool ReadTileSetFile(const std::string& mapFilename, int offset, IsoMap& map);

	static bool ReadTileSetNode(pugi::xml_node root, int offset, IsoMap& map);
};

