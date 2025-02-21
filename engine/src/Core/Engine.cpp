#include <Core/Engine.h>
#include <Graphics/TextureManager.h>
#include <Timer/Timer.h>
#include <raylib/raylib.h>

Engine& Engine::GetInstance()
{
    static Engine m_Instance;
    return m_Instance;
}

bool Engine::Init()
{
    InitWindow (1280, 720, "Sample");
    if (!IsWindowReady()) {
        return false;
    }
    SetTargetFPS (120);

    TextureManager::GetInstance().Load ("mage_idle", "assets/idle.png");
    TextureManager::GetInstance().Load ("mage_run", "assets/run.png");
    graphics::registerPlayerSprites();

    m_world = world::GameWorld::createGameWorld (map::RegisteredMap::level_1);
    if (!m_world.has_value()) {
        return false;
    }

    return m_bIsRunning = true;
}

void Engine::Clean()
{
    TextureManager::GetInstance().Clean();
    CloseWindow();
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
