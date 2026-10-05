#ifndef CAMERA_H
#define CAMERA_H

#include "SDL.h"
#include "../Physics/Point.h"
#include "../Physics/Vector2D.h"
#include "../Core/Engine.h"

class Camera
{
     public:
          void Update(float dt);

          static inline Camera* GetInstance()
          {
               if (s_Instance == nullptr)
                    s_Instance = new Camera();

               return s_Instance;
          }

          inline SDL_Rect GetViewBox() { return m_ViewBox; }
          inline Vector2D GetPosition() { return m_Position; }
          inline void SetTarget(Point* target) { m_Target = target; }

     private:
          Camera() { m_ViewBox = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }; }

          SDL_Rect m_ViewBox;
          Point* m_Target;
          Vector2D m_Position;

          static Camera* s_Instance;
};

#endif // CAMERA_H