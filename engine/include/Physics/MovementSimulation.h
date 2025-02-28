#pragma once
#include <Physics/PhysicsUpdateState.h>
#include <Player/Player.h>
#include <World/Level.h>

namespace physics::movement {

UpdateState computeUpdatedMovementState (float dt, const player::Player& player, const world::GroundData& ground);

UpdateState simulatePhysRunning (const float dt, float simulationTime, const geom::Vector2D& playerVelocity);
UpdateState simulatePhysAirMovement (const float dt, float simulationTime, const geom::Vector2D& playerVelocity);
UpdateState simulatePhysJumping (const float dt, float simulationTime, const geom::Vector2D& playerVelocity);

} // namespace physics::movement
