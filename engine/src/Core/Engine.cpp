#include <Core/Engine.h>
#include <Inputs/InputHandler.h>
#include <Graphics/TextureManager.h>
#include <Timer/Timer.h>
#include <raylib/raylib.h>

#include <Characters/Player.h>
#pragma warning(push, 0)
#include <tinytmx/tinytmx.hpp>
#pragma warning(pop)

#include <iostream>


class APressedCommand : public Command
{
    bool Execute (IObject*)
    {
        std::cout << "Key A is pressed\n";
        return true;
    }
};

static Player* player = nullptr;
static InputLayer* iLayer = nullptr;

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
    SetTargetFPS (60);

    TextureManager::GetInstance().Load ("mage_idle", "assets/idle.png");
    TextureManager::GetInstance().Load ("mage_run", "assets/run.png");
    Properties playerProp ("mage_idle", 128.f, 128.f, phs::Vector2D (100, 100));
    player = new Player (playerProp);

    iLayer = new InputLayer();
    iLayer->AddAction (GAMEPAD_BUTTON_UNKNOWN, KEY_A, ActionType::HOLD, std::make_unique <MageMoveCommand>());
    iLayer->AddAction (GAMEPAD_BUTTON_UNKNOWN, KEY_W, ActionType::HOLD, std::make_unique <MageMoveCommand>());
    iLayer->AddAction (GAMEPAD_BUTTON_UNKNOWN, KEY_S, ActionType::HOLD, std::make_unique <MageMoveCommand>());
    iLayer->AddAction (GAMEPAD_BUTTON_UNKNOWN, KEY_D, ActionType::HOLD, std::make_unique <MageMoveCommand>());
    InputHandler::GlobalInstance().PushInputLayer (iLayer);
    InputHandler::GlobalInstance().PushObject (player);

    tinytmx::Map aMap;
    aMap.ParseFile ("assests/map_test.tmx");
    

    return m_bIsRunning = true;
}

void Engine::Clean()
{
    InputHandler::GlobalInstance().PopInputLayer();
    InputHandler::GlobalInstance().PopObject();
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
    InputHandler::GlobalInstance().HandleInput();
    player->Update (Timer::GlobalInstance().GetDeltaTime());
}

void Engine::Render()
{
    BeginDrawing();
    ClearBackground (GRAY);

    player->Draw();
    EndDrawing();

}

void Engine::HandleEvents()
{
}
