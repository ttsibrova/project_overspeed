#pragma once
#include <Characters/Player.h>
#include <Characters/PlayerStates.h>
#include <Characters/PlayerMovementMode.h>
#include <Physics/PhysicsUpdateState.h>
#include <World/Level.h>


class Player;
namespace PlayerMovement
{

PhysicsUpdateState ComputeUpdatePlayerMovement (float dt, const Player& player, const GroundData& ground);

player::MovementMode SelectMovementModeOnAction (player::Action action, player::MovementMode currentMode);

PhysicsUpdateState SimulatePhysRunning     (const float dt, float simulationTime, const phs::Vector2D& playerVelocity);
PhysicsUpdateState SimulatePhysAirMovement (const float dt, float simulationTime, const phs::Vector2D& playerVelocity);
PhysicsUpdateState SimulatePhysJumping     (const float dt, float simulationTime, const phs::Vector2D& playerVelocity);
}


