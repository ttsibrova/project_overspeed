#pragma once
#include <Physics/PhysicsUpdateState.h>
#include <Player/Player.h>
#include <Player/PlayerPhysicalState.h>
#include <World/Level.h>

namespace physics::movement
{

UpdateState computeUpdatePlayerMovement (float dt, const player::Player& player, const world::GroundData& ground);

player::MovementMode selectMovementModeOnAction (player::Action action, player::MovementMode currentMode);

UpdateState simulatePhysRunning     (const float dt, float simulationTime, const geom::Vector2D& playerVelocity);
UpdateState simulatePhysAirMovement (const float dt, float simulationTime, const geom::Vector2D& playerVelocity);
UpdateState simulatePhysJumping     (const float dt, float simulationTime, const geom::Vector2D& playerVelocity);

}


