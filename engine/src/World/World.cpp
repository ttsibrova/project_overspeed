#include <World/World.h>

#include <Physics/MovementSimulation.h>
#include <Input/InputHandler.h>
#include <Timer/Timer.h>
#include <WorldInteraction/PlayerWithWorldInteraction.h>

#include <print>

#if _DEBUG
#include <Debug/GlobalDebugger.h>
#endif

namespace world {

GameWorld::GameWorld (Level&& lvl):
    m_player(),
    m_currentLevel (std::move (lvl))
{
    m_playerInputLayer.addAction (GAMEPAD_BUTTON_UNKNOWN, KEY_A, input::ActionType::HOLD, player::MovePlayer);
    //m_playerInputLayer.addAction (GAMEPAD_BUTTON_UNKNOWN, KEY_W, input::ActionType::HOLD, PlayerMovement::MovePlayer);
    //m_playerInputLayer.addAction (GAMEPAD_BUTTON_UNKNOWN, KEY_S, input::ActionType::HOLD, PlayerMovement::MovePlayer);
    m_playerInputLayer.addAction (GAMEPAD_BUTTON_UNKNOWN, KEY_D, input::ActionType::HOLD, player::MovePlayer);
    m_playerInputLayer.addAction (GAMEPAD_BUTTON_UNKNOWN, KEY_SPACE, input::ActionType::PRESS, player::JumpPlayer);
}


std::optional<GameWorld> GameWorld::createGameWorld (map::RegisteredMap map)
{
    auto level = Level::createLevel (map);
    if (!level.has_value()) {
        return {};
    }

    return GameWorld (std::move (level.value()));
}

void GameWorld::update()
{
#if _DEBUG
    Debug::GlobalDebugger::GetInstance().update();
#endif
    InputHandler::getInstance().handleInput (m_playerInputLayer, m_player);

    float dt = static_cast <float> (Timer::getInstance().GetDeltaTime());
    auto groundData = m_currentLevel.getGroundData();
    auto newPhysicsState = physics::movement::computeUpdatePlayerMovement (dt, m_player, m_currentLevel.getGroundData());
    m_player.update (newPhysicsState);
    auto newBodyState = interaction::updateBodyStateOnInteraction (m_player.getCollider(player::ColliderType::INTERACTION), m_player.getBodyState(), groundData);
    m_player.update (newBodyState);
}

void GameWorld::draw()
{
    m_currentLevel.draw();
    m_player.draw();

#if _DEBUG
    Debug::GlobalDebugger::GetInstance().draw();
#endif
}

}
