#ifndef INPUT_H
#define INPUT_H

#include "SDL.h"

// Input Handler

class Input
{
     public:
          void Listen();
          bool GetKeyDown(SDL_Scancode key);

          static inline Input* GetInstance()
          {
               if (s_Instance == nullptr)
                    s_Instance = new Input();

               return s_Instance;
          }

     private:
          Input();

          void KeyUp();
          void KeyDown();

          static Input* s_Instance;
          const Uint8* m_KeyStates;
};

#endif // INPUT_H
