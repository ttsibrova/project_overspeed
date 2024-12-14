#include <Timer/Timer.h>

#include <raylib/raylib.h>


Timer::Timer():
    m_deltaTime (0),
    m_lastTime (0)
{}

Timer& Timer::GlobalInstance()
{
    static Timer timer;
    return timer;
}

void Timer::Update()
{
    double currTime = GetTime();
    m_deltaTime = currTime - m_lastTime;
    m_lastTime = currTime;
}



