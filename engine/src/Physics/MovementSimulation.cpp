#include <Physics/MovementSimulation.h>

#include <Input/InputGlobalGetters.h>
#include <Player/Player.h>
#include <Player/PlayerPhysicalState.h>
#include <WorldInteraction/GroundCollision.h>

#include <cassert>
#include <print>

#define MIN_TICK_TIME 1.e-4f
#define MAX_SIM_TICK_TIME 0.02f
#define FUZZY_COMPARE_VALUE 1.e-7f

namespace physics::movement {
namespace {

player::MovementMode resolveMovementModeOnIDLE (player::MovementMode currentMode)
{
    switch (currentMode) {
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

player::MovementMode resolveMovementModeOnMOVE (player::MovementMode currentMode)
{
    switch (currentMode) {
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

player::MovementMode resolveMovementModeOnJUMP (player::MovementMode currentMode)
{
    switch (currentMode) {
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

player::MovementMode updateModeOnConditions (player::MovementMode targetMode, const physics::Collider& playerCollider,
                                             const world::GroundData& ground)
{
    bool isGrounded = Collision::IsPlayerGrounded (playerCollider, ground);
    switch (targetMode) {
    case player::MovementMode::NONE:
    case player::MovementMode::RUNNING:
    case player::MovementMode::JUMPING:
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

player::MovementMode selectMovementModeOnAction (player::Action action, player::MovementMode currentMode)
{
    switch (action) {
    case player::Action::IDLE:
        return resolveMovementModeOnIDLE (currentMode);
    case player::Action::MOVE:
        return resolveMovementModeOnMOVE (currentMode);
    case player::Action::SLIDE:
        break;
    case player::Action::JUMP:
        return resolveMovementModeOnJUMP (currentMode);
    default:
        break;
    }
    assert (false); // should not be there
    return player::MovementMode::NONE;
}

template <class PhysSim, class... PhysArgs>
UpdateState simulatePhys (const player::Player& player, const world::GroundData& ground, PhysSim physSim, PhysArgs... args)
{
    UpdateState physUpdate     = physSim (args...);
    auto        playerCollider = player.getCollider();
    auto        adjustedVec    = Collision::HitScanGround (playerCollider, physUpdate.trsl, ground);
    if (adjustedVec.has_value()) {
        physUpdate.trsl     = adjustedVec.value();
        physUpdate.velocity = geom::Vector2D(); // temporary velocity reset on collision
        return physUpdate;
    }
    return physUpdate;
}

} // namespace

UpdateState computeUpdatedMovementState (float dt, const player::Player& player, const world::GroundData& ground)
{
    auto                 playerState = player.getPhysicalState();
    player::MovementMode targetMode  = selectMovementModeOnAction (playerState.nextAction, playerState.currentMode);
    targetMode                       = updateModeOnConditions (targetMode, player.getCollider(), ground);

    std::vector<player::MovementMode> appliedModes;
    appliedModes.push_back (targetMode);

    float simulationTime = playerState.currentSimTime;
    if (targetMode != playerState.currentMode) {
        simulationTime = 0.f;
    }

    switch (targetMode) {
    case player::MovementMode::NONE:
    {
        UpdateState physicsState {
            .nextMode = targetMode,
            .velocity = geom::Vector2D(),
            .trsl     = geom::Vector2D(),
            .simTime  = dt,
        };
        return physicsState;
    }
    case player::MovementMode::RUNNING:
    {
        return simulatePhys (player, ground, simulatePhysRunning, dt, simulationTime, playerState.velocity);
    }
    case player::MovementMode::AIR_MOVEMENT:
    {
        return simulatePhys (player, ground, simulatePhysAirMovement, dt, simulationTime, playerState.velocity);
    }
    case player::MovementMode::JUMPING:
    {
        return simulatePhys (player, ground, simulatePhysJumping, dt, simulationTime, playerState.velocity);
    }
    }
    float newSimTime = dt;
    if (targetMode == playerState.currentMode) {
        newSimTime += playerState.currentSimTime;
    }
    UpdateState emptyPhysicsUpdate { targetMode, playerState.velocity, geom::Vector2D(), newSimTime };
    return emptyPhysicsUpdate;
}

UpdateState simulatePhysRunning (const float dt, float simulationTime, const geom::Vector2D& playerVelocity)
{
    const float playerMaxSpeed = 400.f;
    const float maxSpeedDelay  = .2f;
    const float startSpeed     = 100.f;
    const float dSpeed         = (playerMaxSpeed - startSpeed) / maxSpeedDelay;

    __assume (simulationTime >= 0.f);
    __assume (dt >= 0.f);

    auto inputVec = geom::stripByAxis (input::getAxisVec(), geom::Axis::X);
    if (inputVec.SquareMagnitude() < 1e-5 || std::abs (inputVec.X()) < 1e-5) {
        return { player::MovementMode::NONE, playerVelocity, geom::Vector2D(), 0. }; // no direction
    }

    geom::Vector2D newVelocity         = geom::stripByAxis (playerVelocity, geom::Axis::X);
    auto           currHorizontalSpeed = newVelocity.Magnitude();
    if (currHorizontalSpeed > FUZZY_COMPARE_VALUE && geom::isOpposite (playerVelocity, inputVec)) {
        newVelocity.FlipX();
    }

    float          tickTime      = std::max (dt, MIN_TICK_TIME);
    float          remainingTime = 0.f;
    geom::Vector2D trsl;

    while (tickTime > 0.f) {
        if (tickTime > MAX_SIM_TICK_TIME) {
            remainingTime = tickTime - MAX_SIM_TICK_TIME;
            tickTime      = MAX_SIM_TICK_TIME;
        }

        float speed = startSpeed + dSpeed * (simulationTime + tickTime);
        speed       = std::min (playerMaxSpeed, speed);

        currHorizontalSpeed = std::min (playerMaxSpeed, currHorizontalSpeed);
        currHorizontalSpeed = std::max (currHorizontalSpeed, speed);

        newVelocity = inputVec * currHorizontalSpeed;
        trsl        = newVelocity * tickTime;
        simulationTime += tickTime;

        if (remainingTime > MAX_SIM_TICK_TIME) {
            remainingTime -= MAX_SIM_TICK_TIME;
        }
        else {
            tickTime      = remainingTime;
            remainingTime = 0.f;
        }
    }

    return { player::MovementMode::RUNNING, newVelocity, trsl, simulationTime };
}

UpdateState simulatePhysAirMovement (const float dt, const float simulationTime, const geom::Vector2D& playerVelocity)
{
    const float gravityAcceleration = 1200.f;
    // const float maxFallSpeed = 500.f;

    float          tickTime         = std::max (dt, MIN_TICK_TIME);
    geom::Vector2D verticalVelocity = geom::stripByAxis (playerVelocity, geom::Axis::Y);
    geom::Vector2D addedVelocity    = geom::getDownVector() * gravityAcceleration * (tickTime);

    verticalVelocity = verticalVelocity + addedVelocity;
    // if (verticalVelocity.SquareMagnitude() > maxFallSpeed * maxFallSpeed) {
    //     verticalVelocity = physics::DownVector() * maxFallSpeed;
    // }
    geom::Vector2D newVelocity (playerVelocity.X(), verticalVelocity.Y());
    geom::Vector2D trsl = newVelocity * tickTime;

    if (newVelocity.Y() > 0.f) {
        return { player::MovementMode::AIR_MOVEMENT, newVelocity, trsl, simulationTime + tickTime };
    }
    else {
        return { player::MovementMode::JUMPING, newVelocity, trsl, simulationTime + tickTime };
    }
}

UpdateState simulatePhysJumping (const float dt, float simulationTime, const geom::Vector2D& playerVelocity)
{
    if (simulationTime == 0.f) {
        return simulatePhysAirMovement (dt, simulationTime, geom::Vector2D (playerVelocity.X(), playerVelocity.Y() - 700.f));
    }
    else {
        return simulatePhysAirMovement (dt, simulationTime, playerVelocity);
    }
}

} // namespace physics::movement
