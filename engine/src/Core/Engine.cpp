#include <Core/Engine.h>
#include <Inputs/InputHandler.h>
#include <Graphics/TextureManager.h>
#include <Timer/Timer.h>
#include <raylib/raylib.h>

#include <Characters/Player.h>
#include <World/Level.h>
#pragma warning(push, 0)
#include <tinytmx/tinytmx.hpp>
#pragma warning(pop)

#include <iostream>


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

    m_world = GameWorld::CreateGameWorld (Maps::level_1);
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

void Engine::Update()
{
    Timer::GlobalInstance().Update();
    if (m_world.has_value()) {
        m_world.value().Update();
    }
}

void Engine::Render()
{
    BeginDrawing();
    ClearBackground (GRAY);
    if (m_world.has_value()) {
        m_world.value().Draw();
    }

    EndDrawing();

}

void Engine::HandleEvents()
{
}
