#pragma once
#include <World/World.h>
#include <Core/Logger.h>
#include <Input/InputHandler.h>

#include <optional>

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

    core::Logger        logger;
    input::InputHandler inputHandler;

private:
    Engine():
        m_bIsRunning (false)
    {}

    bool                            m_bIsRunning;
    std::optional<world::GameWorld> m_world;
};
