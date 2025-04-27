#include <Core/Engine.h>

#include <Graphics/TextureManager.h>
#include <Input/InputHandler.h>
#include <Timer/Timer.h>

#include <raylib.h>

Engine& Engine::getInstance()
{
    static Engine m_Instance;
    return m_Instance;
}

bool Engine::Init()
{
    logger.info ("Launching window...");
    InitWindow (1920, 1080, "Sample");
    if (!IsWindowReady()) {
        return false;
    }
    SetTargetFPS (120);
    logger.info ("Window initalized.");
    logger.info ("Regestering player related sprites...");
    graphics::registerPlayerSprites();
    logger.info ("Sprites loaded");

    logger.info ("Loading game world...");
    try {
        m_world.emplace (world::GameWorldBuilder { map::RegisteredMap::level_1 });
    }
    catch (const std::exception& e) {
        logger.error (e.what());
        return false;
    }
    logger.info ("World loaded successfully. Engine is running.");

    return m_bIsRunning = true;
}

void Engine::clean()
{
    logger.info ("Closing window...");
    TextureManager::getInstance().clean();
    CloseWindow();
    logger.info ("Window closed sucessfuly.");
}

void Engine::Quit()
{
    m_bIsRunning = false;
}

void Engine::update()
{
    Timer::getInstance().update();
    inputHandler.handleInput();
    if (m_world.has_value()) {
        m_world->update();
    }
}

void Engine::Render()
{
    BeginDrawing();
    ClearBackground (Color { 61, 119, 109 });
    if (m_world.has_value()) {
        m_world->draw();
    }

    EndDrawing();
}

void Engine::HandleEvents()
{}
