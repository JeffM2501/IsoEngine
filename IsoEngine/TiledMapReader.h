#pragma once
#include "IsoEngine.h"
#include "IsoMap.h"

class TiledMapReader
{
public:
	static bool Read(const std::string& mapFilename, IsoMap& map);

private:
	static bool ReadTileSet(const std::string& mapFilename, int offset, IsoMap& map);
};

