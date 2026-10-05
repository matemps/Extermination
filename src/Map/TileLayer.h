#ifndef TILELAYER_H
#define TILELAYER_H

#include "Layer.h"
#include <string>
#include <vector>
#include <map>

struct Tileset
{
     int FirstID, LastID;
     int RowCount, ColCount;
     int TileCount, TileSize;
     std::string Name, Source;

     // For image collection tilesets
     std::map<int, std::string> TileImages;
     std::map<int, int> TileWidths;
     std::map<int, int> TileHeights;
};

using TilesetList = std::vector<Tileset>;
using TileMap = std::vector<std::vector<int>>;

class TileLayer : public Layer
{
     public:
          TileLayer(
               int tilesize, int colcount, int rowcount,
               bool iscollidable,
               TileMap tilemap, TilesetList tilesets
          );

          bool GetIsCollidable();
          virtual void Render();
          virtual void Update();
          inline TileMap GetTilemap() { return m_Tilemap; }

     private:
          int m_TileSize;
          int m_RowCount, m_ColCount;
          bool m_IsCollidable;

          TileMap m_Tilemap;
          TilesetList m_Tilesets;
};

#endif // TILELAYER_H
