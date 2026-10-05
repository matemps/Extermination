#ifndef ENGINE_H
#define ENGINE_H

#include "SDL.h"
#include "../Map/GameMap.h"

#define SCREEN_WIDTH 1360
#define SCREEN_HEIGHT 768

class Engine
{
     public:
          void Init();
          void Events();
          void Render();
          void Update();
          void Clean();
          void Quit();

          static inline Engine* GetInstance()
          {
               if (s_Instance == nullptr)
                    s_Instance = new Engine();

               return s_Instance;
          }
          inline SDL_Renderer* GetRenderer() { return m_Renderer; }
          inline GameMap* GetMap() { return m_LevelMap; }
          inline bool IsRunning() { return m_IsRunning; }

     private:
          Engine()
               : m_Window(nullptr),
                 m_Renderer(nullptr),
                 m_LevelMap(nullptr),
                 m_IsRunning(false)
          {}

          static Engine* s_Instance;
          SDL_Window* m_Window;
          SDL_Renderer* m_Renderer;
          GameMap* m_LevelMap;
          bool m_IsRunning;
};

#endif // ENGINE_H