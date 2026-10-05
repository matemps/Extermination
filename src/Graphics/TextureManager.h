#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <string>
#include <map>
#include "SDL.h"

class TextureManager
{
     public:
          static TextureManager* GetInstance()
          {
               if (s_Instance == nullptr)
                    s_Instance = new TextureManager();

               return s_Instance;
          }

          bool Load(std::string id, std::string filename);
          void Drop(std::string id);
          void Clean();

          void Draw(
               std::string id,
               int x, int y, int width, int height,
               SDL_RendererFlip flip = SDL_FLIP_NONE
          );

          void DrawBackground(
               std::string id,
               int width, int height
          );

          void DrawTile(
               std::string tilesetID,
               int tileSize, int x, int y, int row, int col,
               SDL_RendererFlip flip = SDL_FLIP_NONE
          );

          void DrawFrame(
               std::string id,
               int x, int y, int width, int height, int row, int frame,
               SDL_RendererFlip flip = SDL_FLIP_NONE
          );

     private:
          TextureManager() {}

          std::map<std::string, SDL_Texture*> m_TextureMap;
          static TextureManager* s_Instance;
};

#endif // TEXTUREMANAGER_H
