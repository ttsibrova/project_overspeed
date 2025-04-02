#include <World/World.h>

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
    m_currentLevel (std::move (lvl))
{
    input::getInputHandler().registerInputLayer (m_player, m_player.getInputLayer());
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
}

void GameWorld::draw()
{
    m_currentLevel.draw();
    m_player.draw();

#if _DEBUG
    Debug::GlobalDebugger::getInstance().draw();
#endif
}

GameWorld::~GameWorld()
{
    input::getInputHandler().unregisterInputLayers (m_player);
    core::log::warning ("Game World has been deleted");
}

} // namespace world
