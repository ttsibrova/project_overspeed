#include <World/World.h>

#include <Tools/Assert.h>
#include <Input/InputHandler.h>
#include <Physics/MovementSimulation.h>
#include <Timer/Timer.h>
#include <WorldInteraction/PlayerWithWorldInteraction.h>

#include<Core/Logger.h>

#include <exception>

#if _DEBUG
#include <Debug/GlobalDebugger.h>
#endif

namespace world {

GameWorld::GameWorld (Level&& lvl):
    m_player(),
    m_currentLevel (std::move (lvl)),
    m_action (WorldAction::None)
{
    m_camera.target = m_player.getPosition();
    m_camera.offset = Vector2 { 1920 / 2.f, 1080 / 2.f };
    m_camera.rotation = 0.f;
    m_camera.zoom     = 2.f;
    m_inputLayer.addAction (GAMEPAD_BUTTON_UNKNOWN, KEY_E, input::ActionType::PRESS, [this]() { setAction (WorldAction::PlayerInteraction);});
    input::getInputHandler().registerInputLayer (m_player, m_player.getInputLayer());
}

void GameWorld::setAction (WorldAction action)
{
    m_assert (m_action == WorldAction::None, "More than one action is triggered in GameWorld");
    m_action = action;
}

GameWorld GameWorld::createGameWorld (map::RegisteredMap map)
{
    auto level = Level::createLevel (map);
    if (!level.has_value()) {
        throw std::exception { "Failed to initialized world." };
    }

    return GameWorld (std::move (level.value()));
}

void GameWorld::update()
{
#if _DEBUG
    Debug::GlobalDebugger::getInstance().update();
#endif
    input::getInputHandler().handleTargetedInput (m_inputLayer);

    float dt         = static_cast<float> (Timer::getInstance().GetDeltaTime());
    auto  groundData = m_currentLevel.getGroundData();
    auto newPhysicsState = physics::movement::computeUpdatedMovementState (dt, m_player, m_currentLevel.getGroundData());
    m_player.update (newPhysicsState);
    auto newBodyState = interaction::updateBodyStateOnInteraction (m_player.getCollider (player::ColliderType::INTERACTION),
                                                                   m_player.getBodyState(), groundData);
    m_player.update (newBodyState);
    auto interactableTiles = m_currentLevel.getLevelInteractableTiles();
    auto newAction = interaction::detectActionByInteraction (m_player.getCollider(), interactableTiles);
    if (newAction != player::Action::IDLE) {
        m_player.addAction (newAction);
    }
    m_camera.target = m_player.getPosition();
}

void GameWorld::draw()
{
    BeginMode2D(m_camera);
    m_currentLevel.draw();
    m_player.draw();

#if _DEBUG
    Debug::GlobalDebugger::getInstance().draw();
#endif
    EndMode2D();
}

GameWorld::~GameWorld()
{
    input::getInputHandler().unregisterInputLayers (m_player);
    core::log::warning ("Game World has been deleted");
}

} // namespace world
