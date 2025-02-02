#include <World/World.h>

#include <Characters/PlayerMovement.h>
#include <Inputs/InputHandler.h>
#include <Timer/Timer.h>

#include <print>

#if _DEBUG
#include <Debug/GlobalDebugger.h>
#endif

GameWorld::GameWorld (Level&& lvl):
    m_player ({128.f, 128.f}),
    m_currentLevel (std::move (lvl))
{
    m_playerInputLayer.AddAction (GAMEPAD_BUTTON_UNKNOWN, KEY_A, ActionType::HOLD, PlayerMovement::MovePlayer);
    //m_playerInputLayer.AddAction (GAMEPAD_BUTTON_UNKNOWN, KEY_W, ActionType::HOLD, PlayerMovement::MovePlayer);
    //m_playerInputLayer.AddAction (GAMEPAD_BUTTON_UNKNOWN, KEY_S, ActionType::HOLD, PlayerMovement::MovePlayer);
    m_playerInputLayer.AddAction (GAMEPAD_BUTTON_UNKNOWN, KEY_D, ActionType::HOLD, PlayerMovement::MovePlayer);
    m_playerInputLayer.AddAction (GAMEPAD_BUTTON_UNKNOWN, KEY_SPACE, ActionType::PRESS, PlayerMovement::JumpPlayer);
}


std::optional<GameWorld> GameWorld::CreateGameWorld (Maps map)
{
    auto level = Level::CreateLevel (map);
    if (!level.has_value()) {
        return {};
    }

    return GameWorld (std::move (level.value()));
}

void GameWorld::Update()
{
#if _DEBUG
    Debug::GlobalDebugger::GetInstance().Update();
#endif
    InputHandler::GlobalInstance().HandleInput (m_playerInputLayer, m_player);

    float dt = static_cast <float> (Timer::GlobalInstance().GetDeltaTime());
    auto playerUpdateState = PlayerMovement::ComputeUpdatePlayerMovement (dt, m_player, m_currentLevel.GetGroundData());
    m_player.Update (playerUpdateState);
}

void GameWorld::Draw()
{
    m_currentLevel.Draw();
    m_player.Draw();

#if _DEBUG
    Debug::GlobalDebugger::GetInstance().Draw();
#endif
}
