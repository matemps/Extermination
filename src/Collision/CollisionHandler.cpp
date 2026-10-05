#include "CollisionHandler.h"
#include "../Core/Engine.h"
#include "../Map/Layer.h"
#include <iostream>

CollisionHandler* CollisionHandler::s_Instance = nullptr;

CollisionHandler::CollisionHandler()
{
     std::vector<Layer*> mapLayers = Engine::GetInstance()->GetMap()->GetMapLayers();
     for (Layer* layer : mapLayers)
     {
          TileLayer* tileLayer = dynamic_cast<TileLayer*>(layer);
          
          if (tileLayer->GetIsCollidable())
          {
               m_CollisionLayers.push_back(tileLayer);
          }
     }

     std::vector<TileMap> tileMaps;
     for (TileLayer* tileLayer : m_CollisionLayers)
     {
          tileMaps.push_back(tileLayer->GetTilemap());
     }

     int rows = tileMaps[0].size();
     int cols = tileMaps[0][0].size();

     TileMap result(rows, std::vector<int>(cols));

     for (const auto& tileMap : tileMaps)
     {
          for (int i = 0; i < rows; i++)
          {
               for (int j = 0; j < cols; j++)
               {
                    result[i][j] += tileMap[i][j];
               }
          }
     }

     m_CollisionTilemap = result;
}

bool CollisionHandler::CheckCollision(SDL_Rect a, SDL_Rect b)
{
     bool x_overlaps = (a.x < b.x + b.w) && (a.x + a.w > b.x);
     bool y_overlaps = (a.y < b.y + b.h) && (a.y + a.h > b.y);

     return (x_overlaps && y_overlaps);
}

bool CollisionHandler::MapCollision(SDL_Rect a)
{
     int tileSize = 32;
     int rowCount = 20;
     int colCount = 60;

     int left_tile = a.x / tileSize;
     int right_tile = (a.x + a.w) / tileSize;

     int top_tile = a.y / tileSize;
     int bottom_tile = (a.y + a.h) / tileSize;

     if (left_tile < 0) left_tile = 0;
     if (right_tile > colCount) right_tile = colCount;

     if (top_tile < 0) top_tile = 0;
     if (bottom_tile > rowCount) bottom_tile = rowCount;

     for (int i = left_tile; i <= right_tile; ++i)
     {
          for (int j = top_tile; j <= bottom_tile; ++j)
          {
               if (m_CollisionTilemap[j][i] > 0)
               {
                    return true;
               }
          }
     }

     return false;
}