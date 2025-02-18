#pragma once

class Timer
{
public:
    static Timer& getInstance();

    void update();

    inline double GetDeltaTime() const { return m_deltaTime; }

    Timer (const Timer& other) = delete;
    Timer operator=(const Timer& other) = delete;

private:
    Timer();

private:
    double m_deltaTime;
    double m_lastTime;
};

