#include <Characters/PlayerMovement.h>
#include <Characters/Player.h>
#include <Inputs/InputHandler.h>
#include <World/LevelCollision.h>
#include <cassert>
#include <print>

#define MIN_TICK_TIME 1.e-4f
#define MAX_SIM_TICK_TIME 0.02f
#define FUZZY_COMPARE_VALUE 1.e-7f


namespace PlayerMovement {

namespace {

player::MovementMode ResolveMMonIDLE (player::MovementMode currentMode)
{
    switch (currentMode)
    {
    case player::MovementMode::NONE:
        return player::MovementMode::NONE;
    case player::MovementMode::RUNNING:
        return player::MovementMode::NONE;
    case player::MovementMode::JUMPING:
        return player::MovementMode::NONE;
    case player::MovementMode::AIR_MOVEMENT:
        return player::MovementMode::AIR_MOVEMENT;
    }

    assert (false); // should not be there
    return player::MovementMode::NONE;
}

player::MovementMode ResolveMMonMOVE (player::MovementMode currentMode)
{
    switch (currentMode)
    {
    case player::MovementMode::NONE:
        return player::MovementMode::RUNNING;
    case player::MovementMode::RUNNING:
        return player::MovementMode::RUNNING;
    case player::MovementMode::JUMPING:
        return player::MovementMode::JUMPING;
    case player::MovementMode::AIR_MOVEMENT:
        return player::MovementMode::AIR_MOVEMENT;
    }

    assert (false); // should not be there
    return player::MovementMode::NONE;
}

player::MovementMode ResolveMMonJUMP (player::MovementMode currentMode)
{
    switch (currentMode)
    {
    case player::MovementMode::NONE:
        return player::MovementMode::JUMPING;
    case player::MovementMode::RUNNING:
        return player::MovementMode::JUMPING;
    case player::MovementMode::JUMPING:
        return player::MovementMode::JUMPING;
    case player::MovementMode::AIR_MOVEMENT:
        return player::MovementMode::AIR_MOVEMENT;
    }

    assert (false); // should not be there
    return player::MovementMode::NONE;
}


player::MovementMode UpdateModeOnConditions (player::MovementMode targetMode, const phs::Collider& playerCollider, const GroundData& ground)
{
    bool isGrounded = Collision::IsPlayerGrounded (playerCollider, ground);
    switch (targetMode)
    {
    case player::MovementMode::NONE: case player::MovementMode::RUNNING: case player::MovementMode::JUMPING:
        if (!isGrounded) {
            return player::MovementMode::AIR_MOVEMENT;
        }
        break;
    case player::MovementMode::AIR_MOVEMENT:
        if (isGrounded) {
            return player::MovementMode::NONE;
        }
        break;
    }
    return targetMode;
}

}

player::MovementMode SelectMovementModeOnAction (PlayerAction action, player::MovementMode currentMode)
{
    switch (action)
    {
    case PlayerAction::IDLE:
        return ResolveMMonIDLE (currentMode);
    case PlayerAction::MOVE:
        return ResolveMMonMOVE (currentMode);
    case PlayerAction::SLIDE:
        break;
    case PlayerAction::JUMP:
        return ResolveMMonJUMP (currentMode);
    default:
        break;
    }
    assert (false); // should not be there
    return player::MovementMode::NONE;
}

template <class PhysSim, class ... PhysArgs>
player::UpdateState SimulatePhys (const Player& player, const GroundData& ground, player::ColliderMode colliderMode,
                                 PhysSim physSim, PhysArgs ... args)
{
    PhysicsUpdateState physUpdate = physSim (args...);
    auto playerCollider = player.getCollider();
    auto adjustedVec = Collision::HitScanGround (playerCollider, physUpdate.trsl, ground);
    if (!adjustedVec.has_value()) {
        player::ColliderMode newColliderState = colliderMode;
        if (colliderMode == player::ColliderMode::TIGHT) {
            auto fullPlayerCollider = player.getCollider (player::ColliderMode::FULL);
            if (!Collision::IsPlayerCollidesWithGround (fullPlayerCollider, ground)) {
                adjustedVec = Collision::HitScanGround (fullPlayerCollider, physUpdate.trsl, ground);
                if (!adjustedVec.has_value()) {
                    newColliderState = player::ColliderMode::FULL;
                }
            }
        }
        return {newColliderState, std::move (physUpdate)};
    }

    player::ColliderMode newColliderState = colliderMode;
    if (colliderMode == player::ColliderMode::FULL) {
        physUpdate.trsl = adjustedVec.value();
        auto tightPlayerCollider = player.getCollider (player::ColliderMode::TIGHT);
        adjustedVec = Collision::HitScanGround (tightPlayerCollider, physUpdate.trsl, ground);
        if (adjustedVec.has_value()) {
            physUpdate.trsl = adjustedVec.value();
        }
        newColliderState = player::ColliderMode::TIGHT;
    }
    else if (colliderMode == player::ColliderMode::TIGHT)
    {
        physUpdate.trsl = adjustedVec.value();
        physUpdate.velocity = phs::Vector2D(); // Still I think this is a temporary option
    }
    return {newColliderState, std::move (physUpdate)};
}


player::UpdateState ComputeUpdatePlayerMovement (float dt, const Player& player, const GroundData& ground)
{
    auto playerState = player.getState();
    player::MovementMode targetMode = PlayerMovement::SelectMovementModeOnAction (playerState.nextAction, playerState.currentMode);
    targetMode = UpdateModeOnConditions (targetMode, player.getCollider(), ground);

    std::vector <player::MovementMode> appliedModes;
    appliedModes.push_back (targetMode);

    float simulationTime = playerState.currentSimTime;
    if (targetMode != playerState.currentMode)
    {
        simulationTime = 0.f;
    }

    switch (targetMode)
    {
    case player::MovementMode::NONE:
    {
        PhysicsUpdateState physicsState {.nextMode = targetMode,
                                         .velocity = phs::Vector2D(),
                                         .trsl = phs::Vector2D(),
                                         .simTime = dt}; 
        return { playerState.currentColliderMode, physicsState };
    }
    case player::MovementMode::RUNNING:
    {
        return SimulatePhys (player, ground, playerState.currentColliderMode, PlayerMovement::SimulatePhysRunning, dt, simulationTime, playerState.m_velocity);
    }
    case player::MovementMode::AIR_MOVEMENT:
    {
        return SimulatePhys (player, ground, playerState.currentColliderMode, PlayerMovement::SimulatePhysAirMovement, dt, simulationTime, playerState.m_velocity);
    }
    case player::MovementMode::JUMPING:
    {
        return SimulatePhys (player, ground, playerState.currentColliderMode, PlayerMovement::SimulatePhysJumping, dt, simulationTime, playerState.m_velocity);
    }
    }
    float newSimTime = dt;
    if (targetMode == playerState.currentMode) {
        newSimTime += playerState.currentSimTime;
    }
    PhysicsUpdateState emptyPhysicsUpdate {targetMode, playerState.m_velocity, phs::Vector2D(), newSimTime};
    return { playerState.currentColliderMode, std::move (emptyPhysicsUpdate) };
}


PhysicsUpdateState SimulatePhysRunning (const float dt, float simulationTime, const phs::Vector2D& playerVelocity)
{
    const float playerMaxSpeed = 400.f;
    const float maxSpeedDelay = .2f;
    const float startSpeed = 100.f;
    const float dSpeed = (playerMaxSpeed - startSpeed) / maxSpeedDelay;

    __assume (simulationTime >= 0.f);
    __assume (dt >= 0.f);

    auto inputVec = phs::StripByAxis (Input::GetAxisVec(), phs::EAxis::X);
    if (inputVec.SquareMagnitude() < 1e-5 || std::abs (inputVec.X()) < 1e-5) {
        return {player::MovementMode::NONE, playerVelocity, phs::Vector2D(), 0.}; //no direction
    }

    phs::Vector2D newVelocity = phs::StripByAxis (playerVelocity, phs::EAxis::X);
    auto currHorizontalSpeed = newVelocity.Magnitude();
    if (currHorizontalSpeed > FUZZY_COMPARE_VALUE && phs::IsOpposite (playerVelocity, inputVec)) {
        newVelocity.FlipX();
    }

    float tickTime = std::max (dt, MIN_TICK_TIME);
    float remainingTime = 0.f;
    phs::Vector2D trsl;

    while (tickTime > 0.f) {
        if (tickTime > MAX_SIM_TICK_TIME) {
            remainingTime = tickTime - MAX_SIM_TICK_TIME;
            tickTime = MAX_SIM_TICK_TIME;
        }

        float speed = startSpeed + dSpeed * (simulationTime + tickTime);
        speed = std::min (playerMaxSpeed, speed);

        currHorizontalSpeed = std::min (playerMaxSpeed, currHorizontalSpeed);
        currHorizontalSpeed = std::max (currHorizontalSpeed, speed);

        newVelocity = inputVec * currHorizontalSpeed;
        trsl = newVelocity * tickTime;
        simulationTime += tickTime;

        if (remainingTime > MAX_SIM_TICK_TIME) {
            remainingTime -= MAX_SIM_TICK_TIME;
        } else {
            tickTime = remainingTime;
            remainingTime = 0.f;
        }
    }

    return {player::MovementMode::RUNNING, newVelocity, trsl, simulationTime};
}

PhysicsUpdateState SimulatePhysAirMovement (const float dt, const float simulationTime, const phs::Vector2D& playerVelocity)
{
    const float gravityAcceleration = 1200.f;
    //const float maxFallSpeed = 500.f;

    float tickTime = std::max (dt, MIN_TICK_TIME);
    phs::Vector2D verticalVelocity = phs::StripByAxis (playerVelocity, phs::EAxis::Y);
    phs::Vector2D addedVelocity = phs::GetDownVector() * gravityAcceleration * (tickTime);
    verticalVelocity = verticalVelocity + addedVelocity;
    //if (verticalVelocity.SquareMagnitude() > maxFallSpeed * maxFallSpeed) {
    //    verticalVelocity = phs::DownVector() * maxFallSpeed;
    //}
    phs::Vector2D newVelocity (playerVelocity.X(), verticalVelocity.Y());
    phs::Vector2D trsl = newVelocity * tickTime;

    if (newVelocity.Y() > 0.f) {
        return {player::MovementMode::AIR_MOVEMENT, newVelocity, trsl, simulationTime + tickTime};
    } else {
        return {player::MovementMode::JUMPING, newVelocity, trsl, simulationTime + tickTime};
    }
}

PhysicsUpdateState SimulatePhysJumping (const float dt, float simulationTime, const phs::Vector2D& playerVelocity)
{
    if (simulationTime == 0.f) {
        return SimulatePhysAirMovement (dt, simulationTime, phs::Vector2D (playerVelocity.X(), playerVelocity.Y() - 700.f));
    } else return SimulatePhysAirMovement (dt, simulationTime, playerVelocity);
}

}
