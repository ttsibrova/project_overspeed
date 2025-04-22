#pragma once
#include <Physics/PhysicsUpdateState.h>
#include <Player/Player.h>
#include <World/Level.h>

namespace physics::movement {

UpdateState computeUpdatedMovementState (float dt, const player::Player& player, const world::GroundData& ground);

} // namespace physics::movement
