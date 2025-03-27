#include <Core/Engine.h>
#include <Graphics/TextureManager.h>
#include <Timer/Timer.h>
#include <raylib/raylib.h>

Engine& Engine::getInstance()
{
    static Engine m_Instance;
    return m_Instance;
}

bool Engine::Init()
{
    m_logger.info ("Launching window...");
    InitWindow (1280, 720, "Sample");
    if (!IsWindowReady()) {
        return false;
    }
    SetTargetFPS (120);
    m_logger.info ("Window initalized.");
    m_logger.info ("Regestering player related sprites...");
    graphics::registerPlayerSprites();
    m_logger.info ("Sprites loaded");

    m_logger.info ("Loading game world...");
    m_world = world::GameWorld::createGameWorld (map::RegisteredMap::level_1);
    if (!m_world.has_value()) {
        m_logger.error ("Failed to load game world. Exiting..");
        return false;
    }
    m_logger.info ("World loaded successfully. Engine is running.");

    return m_bIsRunning = true;
}

void Engine::Clean()
{
    m_logger.info ("Closing window...");
    TextureManager::getInstance().Clean();
    CloseWindow();
    m_logger.info ("Window closed sucessfuly.");
}

void Engine::Quit()
{
    m_bIsRunning = false;
}

void Engine::update()
{
    Timer::getInstance().update();
    if (m_world.has_value()) {
        m_world.value().update();
    }
}

void Engine::Render()
{
    BeginDrawing();
    ClearBackground (Color { 61, 119, 109 });
    if (m_world.has_value()) {
        m_world.value().draw();
    }

    EndDrawing();
}

void Engine::HandleEvents()
{}
