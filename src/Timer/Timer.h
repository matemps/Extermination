#ifndef TIMER_H
#define TIMER_H

const int TARGET_FPS = 60;
const float TARGET_DELTATIME = 1.5f;

class Timer
{
     public:
          void Tick();

          inline float GetDeltaTime() { return m_DeltaTime; }
          static inline Timer* GetInstance()
          {
               if (s_Instance == nullptr)
                    s_Instance = new Timer();

               return s_Instance;
          }

     private:
          Timer()
               : m_DeltaTime(0.0f),
                 m_LastTime(0.0f)
          {}

          static Timer* s_Instance;
          float m_DeltaTime;
          float m_LastTime;
};

#endif // TIMER_H