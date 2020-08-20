#include "TiledMapReader.h"
#include "ResourceManager.h"


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

    MapTypes mapType = MapTypes::Isometric;
    if (orient == "orthogonal")
        mapType = MapTypes::Orthographic;

    int width = root.attribute("width").as_int();
    int height = root.attribute("height").as_int();

    map.SetMapSize(width, height, mapType);

	int tilewidth = root.attribute("tilewidth").as_int();
	int tileheight = root.attribute("tileheight").as_int();

    map.SetTileSize(tilewidth, tileheight);
    map.ClearMap();

    for (auto child : root.children())
    {
        std::string childName = child.name();
        if (childName == "tileset")
        {
            int idOffset = 0;
            if (!child.attribute("firstgid").empty())
                idOffset = child.attribute("firstgid").as_int();

            std::string tilesetFile = child.attribute("source").as_string();
            if (tilesetFile.size() == 0)
            {
                if (!ReadTileSetNode(child, idOffset, map))
                    return false;
            }
            else if (!ReadTileSetFile(ResourceManager::GetRelativeResource(mapFilename, tilesetFile), idOffset, map))
                return false;
        }
        else if (childName == "layer")
        {
            int layerID = child.attribute("id").as_int();

            std::string name = child.attribute("name").as_string();

			if (map.FirstLayer < 0)
				map.FirstLayer = layerID;

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
                    pos.x = 0;
                    do
					{
                        size_t nextDelim = colText.find_first_of(',', charPos);
                        if (nextDelim == std::string::npos || nextDelim == colText.size()-1)
                            nextDelim = colText.size();


                        std::string valStr = colText.substr(charPos, nextDelim - charPos);
                        int val = std::atoi(valStr.c_str());
                        charPos = nextDelim + 1;

                        map.SetLayerTile(layerID, pos, val);

                        pos.x++;

                    } while (charPos <= colText.size());
                    pos.y++;
                } while (linePos < contents.size() && linePos != std::string::npos);
            }
        }
    }

    return true;
}

static void ReadImageData(int& width, int& height, std::string& source, pugi::xml_node image)
{
	width = image.attribute("width").as_int();
	height = image.attribute("height").as_int();

	source = image.attribute("source").as_string();

	if (source.size() > 0)
	{
		if (source[0] == '.')
		{
			size_t firstSlash = source.find_first_of('/');
			if (firstSlash != std::string::npos)
				source = source.substr(firstSlash + 1);
		}
	}
}

bool TiledMapReader::ReadTileSetNode(pugi::xml_node root, int idOffset, IsoMap& map)
{
	int tileWidth = root.attribute("tilewidth").as_int();
	int tileHeight = root.attribute("tileheight").as_int();

	int tileCount = root.attribute("tilecount").as_int();

    int columCount = root.attribute("columns").as_int();
    int spacing = root.attribute("spacing").as_int();
    int margin = root.attribute("margin").as_int();

	for (pugi::xml_node child : root.children())
	{
		std::string n = child.name();
		if (n == "tile")
		{
			int id = child.attribute("id").as_int();

            int width, height;
            std::string source;
            ReadImageData(width, height, source, child.child("image"));

            if (source.size() > 0)
			    map.AddTileSetTile(id + idOffset, source);
		}
        else if (n == "image")
        {
			int width, height;
			std::string source;
			ReadImageData(width, height, source, child);

            int rows = tileCount / columCount;
            int id = 0;
            for (int y = 0; y < rows; y++)
            {
                for (int x = 0; x < columCount; x++)
                {
                    sf::IntRect rect(x * tileWidth + (x * spacing) + margin, y * tileHeight + (y * spacing) + margin, tileWidth, tileHeight);
                    map.AddTileSetTile(id + idOffset, source, rect);
                    id++;
                }
            }
        }
	}

	return true;
}

bool TiledMapReader::ReadTileSetFile(const std::string& tilesetFileName, int idOffset, IsoMap& map)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(tilesetFileName.c_str());

	if (result.status != pugi::xml_parse_status::status_ok)
		return false;

	pugi::xml_node root = doc.child("tileset");

    return ReadTileSetNode(root, idOffset, map);
}
