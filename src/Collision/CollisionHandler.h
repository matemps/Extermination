#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include "SDL.h"
#include "../Map/TileLayer.h"
#include "../Map/GameMap.h"
#include <vector>

class CollisionHandler
{
     public:
          bool CheckCollision(SDL_Rect a, SDL_Rect b);
          bool MapCollision(SDL_Rect a);

          static inline CollisionHandler* GetInstance()
          {
               if (s_Instance == nullptr)
                    s_Instance = new CollisionHandler();

               return s_Instance;
          }

     private:
          CollisionHandler();

          TileMap m_CollisionTilemap;
          std::vector<TileLayer*> m_CollisionLayers;
          static CollisionHandler* s_Instance;
};

#endif // COLLISIONHANDLER_H