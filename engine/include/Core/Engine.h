#pragma once
#include <World/World.h>
#include <Core/Logger.h>

class Engine
{
public:
    static Engine& getInstance();

    bool Init();
    void Clean();
    void Quit();

    void update();
    void Render();
    void HandleEvents();

    bool IsRunning() { return m_bIsRunning; }

    Engine (const Engine&)           = delete;
    Engine operator= (const Engine&) = delete;

    core::Logger& getLogger() { return m_logger; }

private:
    Engine():
        m_bIsRunning (false)
    {}

    bool                            m_bIsRunning;
    std::optional<world::GameWorld> m_world;
    core::Logger                    m_logger;
};
