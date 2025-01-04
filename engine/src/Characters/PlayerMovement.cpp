#include <Characters/PlayerMovement.h>
#include <Characters/Player.h>
#include <Inputs/InputHandler.h>
#include <World/LevelCollision.h>
#include <cassert>
#include <print>

#define MIN_TICK_TIME 1.e-4f
#define FUZZY_COMPARE_VALUE 1.e-7


namespace PlayerMovement {

namespace {

MovementMode ResolveMMonIDLE (MovementMode currentMode)
{
    switch (currentMode)
    {
    case MovementMode::NONE:
        return MovementMode::NONE;
    case MovementMode::RUNNING:
        return MovementMode::NONE;
    case MovementMode::FALLING:
        return MovementMode::FALLING;
    }

    assert (false); // should not be there
    return MovementMode::NONE;
}

MovementMode ResolveMMonMOVE (MovementMode currentMode)
{
    switch (currentMode)
    {
    case MovementMode::NONE:
        return MovementMode::RUNNING;
    case MovementMode::RUNNING:
        return MovementMode::RUNNING;
    case MovementMode::FALLING:
        return MovementMode::FALLING;
    }

    assert (false); // should not be there
    return MovementMode::NONE;
}


MovementMode UpdateModeOnConditions (MovementMode targetMode, const Collider& playerCollider, const GroundData& ground)
{
    bool isGrounded = Collision::IsPlayerGrounded (playerCollider, ground);
    switch (targetMode)
    {
    case MovementMode::NONE: case MovementMode::RUNNING:
        if (!isGrounded) {
            return MovementMode::FALLING;
        }
        break;
    case MovementMode::FALLING:
        if (isGrounded) {
            return MovementMode::NONE;
        }
        break;
    }
    return targetMode;
}

}

MovementMode SelectMovementModeOnAction (PlayerAction action, MovementMode currentMode)
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
        break;
    default:
        break;
    }
    assert (false); // should not be there
    return MovementMode::NONE;
}

PhysicsUpdateState ComputeUpdatePlayerMovement (float dt, const Player& player, const GroundData& ground)
{
    auto playerState = player.GetMovementState();
    MovementMode targetMode = PlayerMovement::SelectMovementModeOnAction (playerState.nextAction, playerState.currentMode);
    auto playerCollider = player.GetCollider();
    targetMode = UpdateModeOnConditions (targetMode, playerCollider, ground);

    switch (targetMode)
    {
    case MovementMode::NONE:
        break;
    case MovementMode::RUNNING:
    {
        auto physUpdate = PlayerMovement::SimulatePhysRunning (dt, playerState.currentSimTime, playerState.m_velocity);
        auto adjustedVec = Collision::ComputeCollisionWithGround (playerCollider, physUpdate.trsf.GetTranslationPart(), ground);
        if (adjustedVec.has_value()) {
            physUpdate.trsf.SetTranslation (adjustedVec.value());
        }
        return physUpdate;
    }
    case MovementMode::FALLING:
    {
        auto physUpdate = PlayerMovement::SimulatePhysFalling (dt, playerState.currentSimTime, playerState.m_velocity);
        auto adjustedVec = Collision::ComputeCollisionWithGround (playerCollider, physUpdate.trsf.GetTranslationPart (), ground);
        if (adjustedVec.has_value ()) {
            physUpdate.trsf.SetTranslation (adjustedVec.value());
        }
        return physUpdate;
        break;
    }
    }
    float newSimTime = dt;
    if (targetMode == playerState.currentMode) {
        newSimTime += playerState.currentSimTime;
    }
    PhysicsUpdateState emptyUpdate {targetMode, playerState.m_velocity, phs::Trsf2D(), newSimTime};
    return emptyUpdate;
}


PhysicsUpdateState SimulatePhysRunning (const float dt, const float simulationTime, const phs::Vector2D& playerVelocity)
{
    const float playerMaxSpeed = 400.f;
    const float maxSpeedDelay = .2f;
    const float startSpeed = 100.f;
    const float dSpeed = (playerMaxSpeed - startSpeed) / maxSpeedDelay;

     (simulationTime >= 0);
     __assume (dt >= 0);

    auto inputVec = phs::StripByAxis (Input::GetAxisVec(), phs::EAxis::X);
    if (inputVec.SquareMagnitude() < 1e-5 || std::abs (inputVec.X()) < 1e-5) {
        return {MovementMode::NONE, playerVelocity, phs::Trsf2D(), 0.}; //no direction
    }

    float tickTime = std::max (dt, MIN_TICK_TIME);
    float speed = startSpeed + dSpeed * (simulationTime + tickTime);
    speed = std::min (playerMaxSpeed, speed);

    phs::Vector2D newVelocity = phs::StripByAxis (playerVelocity, phs::EAxis::X);
    auto currHorizontalSpeed = newVelocity.Magnitude();
    if (currHorizontalSpeed > FUZZY_COMPARE_VALUE && phs::IsOpposite (playerVelocity, inputVec)) {
        newVelocity.FlipX();
    }

    currHorizontalSpeed = std::min (playerMaxSpeed, currHorizontalSpeed);
    currHorizontalSpeed = std::max (currHorizontalSpeed, speed);

    newVelocity = inputVec * currHorizontalSpeed;

    phs::Trsf2D trsl;
    trsl.AddTranslation (newVelocity * tickTime);

    return {MovementMode::RUNNING, newVelocity, trsl, simulationTime + tickTime};
}

PhysicsUpdateState SimulatePhysFalling (const float dt, const float simulationTime, const phs::Vector2D& playerVelocity)
{
    const float gravityAcceleration = 10.f;
    const float maxFallSpeed = 500.f;

    float tickTime = std::max (dt, MIN_TICK_TIME);
    phs::Vector2D verticalVelocity = phs::StripByAxis (playerVelocity, phs::EAxis::Y);
    phs::Vector2D addedVelocity = phs::DownVector() * gravityAcceleration * (simulationTime + tickTime);
    verticalVelocity = verticalVelocity + addedVelocity;
    if (verticalVelocity.SquareMagnitude() > maxFallSpeed * maxFallSpeed) {
        verticalVelocity = phs::DownVector() * maxFallSpeed;
    }
    phs::Vector2D newVelocity (playerVelocity.X(), verticalVelocity.Y());
    phs::Trsf2D trsl;
    trsl.AddTranslation (newVelocity * tickTime);

    return {MovementMode::FALLING, newVelocity, trsl, simulationTime + tickTime};
}

}