#include "MapParser.h"
#include "../Vendor/TinyXML/tinyxml.h"

MapParser* MapParser::s_Instance = nullptr;

bool MapParser::Load()
{
     return Parse("MAP", "assets/maps/map.tmx");
}

bool MapParser::Parse(std::string id, std::string source)
{
     TiXmlDocument xml;
     xml.LoadFile(source);

     if (xml.Error())
     {
          std::cerr << "Failed to load: " << source << std::endl;
          return false;
     }

     TiXmlElement* root = xml.RootElement();
     int rowcount, colcount, tilesize = 0;

     root->Attribute("width", &colcount);
     root->Attribute("height", &rowcount);
     root->Attribute("tilewidth", &tilesize);

     // Parse Tile sets
     TilesetList tilesets;
     for (TiXmlElement* e = root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
     {
          if (e->Value() == std::string("tileset"))
          {
               tilesets.push_back(ParseTileset(e));
          }
     }

     // Parse Layers
     GameMap* gamemap = new GameMap();
     for (TiXmlElement* e = root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
     {
          if (e->Value() == std::string("layer"))
          {
               TileLayer* tilelayer = ParseTileLayer(e, tilesets, tilesize, rowcount, colcount);
               gamemap->m_MapLayers.push_back(tilelayer);
          }
     }

     m_MapDict[id] = gamemap;
     return true;
}

Tileset MapParser::ParseTileset(TiXmlElement* xmlTileset)
{
     Tileset tileset;
     tileset.Name = xmlTileset->Attribute("name");
     xmlTileset->Attribute("firstgid", &tileset.FirstID);

     xmlTileset->Attribute("tilecount", &tileset.TileCount);
     tileset.LastID = (tileset.FirstID + tileset.TileCount) - 1;

     xmlTileset->Attribute("columns", &tileset.ColCount);
     xmlTileset->Attribute("tilewidth", &tileset.TileSize);

     // Avoid division by zero for image collection tilesets
     tileset.RowCount = tileset.ColCount > 0 
          ? tileset.TileCount / tileset.ColCount
          : 0;

     //
     // CASE 1:
     // Standard tileset with a single image
     //
     TiXmlElement* image = xmlTileset->FirstChildElement("image");
     if (image != nullptr)
     {
          tileset.Source = image->Attribute("source");
     }
     else
     {
          //
          // CASE 2:
          // Image collection tileset
          //
          for (TiXmlElement* tile = xmlTileset->FirstChildElement("tile"); tile != nullptr; tile = tile->NextSiblingElement("tile"))
          {
               int id = 0;
               tile->Attribute("id", &id);

               image = tile->FirstChildElement("image");

               if (image != nullptr)
               {
                    std::string source = image->Attribute("source");

                    tileset.TileImages[id] = source;

                    int width = 0;
                    int height = 0;

                    image->Attribute("width", &width);
                    image->Attribute("height", &height);

                    tileset.TileWidths[id] = width;
                    tileset.TileHeights[id] = height;
               }
          }
     }

     return tileset;
}

TileLayer* MapParser::ParseTileLayer(TiXmlElement* xmlLayer, TilesetList tilesets, int tilesize, int rowcount, int colcount)
{
     bool iscollidable = false;

     TiXmlElement* properties = xmlLayer->FirstChildElement("properties");
     if (properties)
     {
          TiXmlElement* property = properties->FirstChildElement("property");

          while (property)
          {
               const char* name = property->Attribute("name");
               const char* value = property->Attribute("value");

               if (name && value && std::string(name) == "IsCollidable")
               {
                    iscollidable = (std::string(value) == "true");
                    break;
               }

               property = property->NextSiblingElement("property");
          }
     }

     TiXmlElement* data = xmlLayer->FirstChildElement("data");

     // Parse Layer tile map
     std::string matrix(data->GetText());
     std::istringstream iss(matrix);
     std::string id;

     TileMap tilemap(rowcount, std::vector<int>(colcount, 0));

     for (int row = 0; row < rowcount; row++)
     {
          for (int col = 0; col < colcount; col++)
          {
               getline(iss, id, ',');
               std::stringstream convertor(id);
               convertor >> tilemap[row][col];

               if (!iss.good())
                    break;
          }
     }

     return (new TileLayer(tilesize, colcount, rowcount, iscollidable, tilemap, tilesets));
}

void MapParser::Clean()
{
     std::map<std::string, GameMap*>::iterator it;
     for (it = m_MapDict.begin(); it != m_MapDict.end(); it++)
     {
          it->second = nullptr;
     }

     m_MapDict.clear();
}
