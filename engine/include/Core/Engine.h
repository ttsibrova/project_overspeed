#pragma once


class Engine
{
public:
    static Engine& GetInstance();

    bool Init();
    void Clean();
    void Quit();

    void Update();
    void Render();
    void HandleEvents();

    bool IsRunning() { return m_bIsRunning; }

    Engine (const Engine&) = delete;
    Engine operator= (const Engine&) = delete;

private:
    Engine():
        m_bIsRunning (false)
    {}

    bool m_bIsRunning;
};
