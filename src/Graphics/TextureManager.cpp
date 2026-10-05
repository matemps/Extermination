#include "TextureManager.h"
#include "../Core/Engine.h"
#include "../Camera/Camera.h"
#include <string>
#include "SDL.h"
#include "SDL_image.h"

TextureManager* TextureManager::s_Instance = nullptr;

bool TextureManager::Load(std::string id, std::string filename)
{
     SDL_Surface* surface = IMG_Load(filename.c_str());
     if (surface == nullptr)
     {
          SDL_Log("Failed to load texture: %s, %s", filename.c_str(), SDL_GetError());
          return false;
     }

     SDL_Texture* texture = SDL_CreateTextureFromSurface(
          Engine::GetInstance()->GetRenderer(), surface
     );
     if (texture == nullptr)
     {
          SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
          return false;
     }

     m_TextureMap[id] = texture;

     return true;
}

void TextureManager::Draw(
     std::string id,
     int x, int y, int width, int height,
     SDL_RendererFlip flip
)
{
     SDL_Rect srcRect = { 0, 0, width, height };

     Vector2D cam = Camera::GetInstance()->GetPosition();
     SDL_Rect dstRect = { x - cam.X, y - cam.Y, width, height };

     SDL_RenderCopyEx(
          Engine::GetInstance()->GetRenderer(),
          m_TextureMap[id], &srcRect, &dstRect,
          0, nullptr, flip
     );
}

void TextureManager::DrawBackground(
     std::string id,
     int width, int height
)
{
     Vector2D cam = Camera::GetInstance()->GetPosition();

     // Get camera offsets w/ scalar for background scrolling
     int offsetX = cam.X * 0.1;
     int offsetY = cam.Y * 0.1;

     // Keep offsets within texture size
     offsetX %= width;
     offsetY %= height;

     // Negative correction
     if (offsetX < 0) offsetX += width;
     if (offsetY < 0) offsetY += height;

     // Start drawing one tile earlier
     int startX = -offsetX;
     int startY = -offsetY;

     for (int y = startY; y < SCREEN_HEIGHT; y += height)
     {
          for (int x = startX; x < SCREEN_WIDTH; x += width)
          {
               SDL_Rect dstRect =
               {
                   x,
                   y,
                   width,
                   height
               };

               SDL_RenderCopy(Engine::GetInstance()->GetRenderer(),
                    m_TextureMap[id], NULL, &dstRect);
          }
     }
}

void TextureManager::DrawTile(
     std::string tilesetID,
     int tileSize, int x, int y, int row, int col,
     SDL_RendererFlip flip
)
{
     SDL_Rect srcRect = {
          tileSize * col, tileSize * row, tileSize, tileSize
     };

     Vector2D cam = Camera::GetInstance()->GetPosition();
     SDL_Rect dstRect = { x - cam.X, y - cam.Y, tileSize, tileSize };

     SDL_RenderCopyEx(
          Engine::GetInstance()->GetRenderer(),
          m_TextureMap[tilesetID],
          &srcRect, &dstRect, 0, 0,
          flip
     );
}

void TextureManager::DrawFrame(
     std::string id,
     int x, int y, int width, int height, int row, int frame,
     SDL_RendererFlip flip
)
{
     SDL_Rect srcRect = {
          width * frame, height * (row - 1), width, height
     };

     Vector2D cam = Camera::GetInstance()->GetPosition();
     SDL_Rect dstRect = { x - cam.X, y - cam.Y, width, height };

     SDL_RenderCopyEx(
          Engine::GetInstance()->GetRenderer(),
          m_TextureMap[id], &srcRect, &dstRect,
          0, nullptr, flip
     );
}

void TextureManager::Drop(std::string id)
{
     SDL_DestroyTexture(m_TextureMap[id]);
     m_TextureMap.erase(id);
}

void TextureManager::Clean()
{
     std::map<std::string, SDL_Texture*>::iterator it;
     for (it = m_TextureMap.begin(); it != m_TextureMap.end(); it++)
     {
          SDL_DestroyTexture(it->second);
     }

     m_TextureMap.clear();

     SDL_Log("TextureMap cleaned.");
}