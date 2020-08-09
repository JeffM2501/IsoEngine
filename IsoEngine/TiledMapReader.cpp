#include "TiledMapReader.h"
#include "ResourceManager.h"
#include "PUGIXML/pugixml.hpp"

bool TiledMapReader::Read(const std::string& mapFilename, IsoMap& map)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(ResourceManager::GetResourcePath(mapFilename).c_str());

    if (result.status != pugi::xml_parse_status::status_ok)
        return false;

    auto root = doc.child("map");

    auto version = root.attribute("version");
    auto tiledVersion = root.attribute("tiledversion");
    
    std::string orient = root.attribute("orientation").as_string();
    std::string renderorder = root.attribute("renderorder").as_string();

    if (orient != "isometric" || renderorder != "right-down")
        return false;

    int width = root.attribute("width").as_int();
    int height = root.attribute("height").as_int();

    map.SetMapSize(width, height);

	int tilewidth = root.attribute("tilewidth").as_int();
	int tileheight = root.attribute("tileheight").as_int();

    map.SetTileSize(tilewidth, tileheight);
    map.ClearMap();

    for (auto child : root.children())
    {
        std::string childName = child.name();
        if (childName == "tileset")
        {
            std::string tilesetFile = child.attribute("source").as_string();
            if (!ReadTileSet(ResourceManager::GetRelativeResource(mapFilename, tilesetFile), map))
                return false;
        }
        else if (childName == "layer")
        {
            int layerID = child.attribute("id").as_int();

            auto data = child.child("data");
            std::string encoding = data.attribute("encoding").as_string();
            if (encoding == "csv")
            {
                std::string contents = data.first_child().value();
                // if it's a windows file it will have \r\n for newlines, not just \n 
                // so remove all instances of \r so that we can consistently read based on \n
                contents.erase(std::remove(contents.begin(), contents.end(), '\r'), contents.end());

                std::vector<std::vector<int>> rawData;

                sf::Vector2i pos;
                size_t linePos = 0;
                do 
                {
                    size_t nextLine = contents.find_first_of('\n', linePos);
                    if (nextLine == std::string::npos)
                        nextLine = contents.size();

                    std::string colText = contents.substr(linePos, nextLine - linePos);
                    linePos = nextLine + 1;

                    if (colText.size() == 0)
                        continue;

                    size_t charPos = 0;
                    pos.y = 0;
                    do
					{
                        size_t nextDelim = colText.find_first_of(',', charPos);
                        if (nextDelim == std::string::npos)
                            nextDelim = colText.size();

                        int val = std::atoi(colText.c_str() + charPos);
                        charPos = nextDelim + 1;

                        map.SetLayerTile(layerID, pos, val - 1);

                        pos.y++;

                    } while (charPos <= colText.size());
                    pos.x++;
                } while (linePos < contents.size() && linePos != std::string::npos);
            }
        }
    }

    return true;
}

bool TiledMapReader::ReadTileSet(const std::string& tilesetFileName, IsoMap& map)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(tilesetFileName.c_str());

	if (result.status != pugi::xml_parse_status::status_ok)
		return false;

	auto root = doc.child("tileset");

	int tilewidth = root.attribute("tilewidth").as_int();
	int tileheight = root.attribute("tileheight").as_int();

    int tileCount = root.attribute("tilecount").as_int();

	for (auto child : root.children())
	{
		std::string n = child.name();
		if (n == "tile")
		{
            int id = child.attribute("id").as_int();

            auto image = child.child("image");
			int width = image.attribute("width").as_int();
			int height = image.attribute("height").as_int();

            std::string source = image.attribute("source").as_string();

            map.AddTileSetTile(id, ResourceManager::ForceFilePath("Tiles",source));
		}
	}

    return true;
}
