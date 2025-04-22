#include <Physics/MovementSimulation.h>

#include <Debug/DebugLog.h>
#include <Input/InputGlobalGetters.h>
#include <Player/Player.h>
#include <Player/PlayerPhysicalState.h>
#include <WorldInteraction/GroundCollision.h>
#include <Tools/Assert.h>

#define MIN_TICK_TIME 1.e-4f
#define MAX_SIM_TICK_TIME 0.02f
#define FUZZY_COMPARE_VALUE 1.e-7f


namespace physics::movement {
namespace {

#pragma region MovementStateMachine
using Mode = player::MovementMode;

player::MovementMode selectMovementModeOnAction (player::Action action, player::MovementMode currentMode)
{
    switch (currentMode) {
    case Mode::NONE:
        switch (action) {
        case player::Action::IDLE : return Mode::NONE;
        case player::Action::MOVE : return Mode::RUNNING;
        case player::Action::SLIDE: return Mode::NONE;    // temporary
        case player::Action::JUMP : return Mode::JUMPING;
        default   : assert (false); return Mode::NONE;
        };
    case Mode::RUNNING:
        switch (action) {
        case player::Action::IDLE : return Mode::NONE;
        case player::Action::MOVE : return Mode::RUNNING;
        case player::Action::SLIDE: return Mode::NONE;  // temporary
        case player::Action::JUMP : return Mode::JUMPING;
        default   : assert (false); return Mode::NONE;
        }
    case Mode::JUMPING: //
        return Mode::JUMPING; // disallows switching to other modes util simulation is done
    case Mode::AIR_MOVEMENT: //
        return Mode::AIR_MOVEMENT; // disallows switching to other modes util simulation is done
    default:
        assert (false); // should not be there
        return Mode::NONE;
    }
}

player::MovementMode updateModeOnConditions (player::MovementMode     targetMode,
                                             const physics::Collider& playerCollider,
                                             const world::GroundData& ground)
{
    bool isGrounded = Collision::IsPlayerGrounded (playerCollider, ground);
    switch (targetMode) {
    case Mode::NONE:
    case Mode::RUNNING:
        if (!isGrounded) {
            return Mode::AIR_MOVEMENT;
        }
        break;
    case Mode::JUMPING: break;
    case Mode::AIR_MOVEMENT:
        if (isGrounded) {
            return Mode::NONE;
        }
        break;
    }
    return targetMode;
}

#pragma endregion

#pragma region Simulations

UpdateState simulatePhysRunning (const float dt, float simulationTime, const geom::Vector2D& playerVelocity)
{
    const float playerMaxSpeed = 600.f;
    const float maxSpeedDelay  = .2f;
    const float startSpeed     = 100.f;
    const float dSpeed         = (playerMaxSpeed - startSpeed) / maxSpeedDelay;

    __assume (simulationTime >= 0.f);
    __assume (dt >= 0.f);

    const auto inputVec = geom::stripByAxis (input::getAxisVec(), geom::Axis::X);
    if (inputVec.getSquareMagnitude() < 1e-5 || std::abs (inputVec.X()) < 1e-5) {
        return { Mode::NONE, playerVelocity, geom::Vector2D(), 0. }; // no direction
    }

    geom::Vector2D newVelocity         = geom::stripByAxis (playerVelocity, geom::Axis::X);
    auto           currHorizontalSpeed = newVelocity.getMagnitude();
    if (currHorizontalSpeed > FUZZY_COMPARE_VALUE && geom::isOpposite (playerVelocity, inputVec)) {
        newVelocity.flipX();
    }

    const float tickTime = std::max (dt, MIN_TICK_TIME);
    float speed = startSpeed + dSpeed * (simulationTime + tickTime);
    speed       = std::min (playerMaxSpeed, speed);

    currHorizontalSpeed = std::min (playerMaxSpeed, currHorizontalSpeed);
    currHorizontalSpeed = std::max (currHorizontalSpeed, speed);

    newVelocity = inputVec * currHorizontalSpeed;
    const geom::Vector2D trsl = newVelocity * tickTime;
    simulationTime += tickTime;

    return { Mode::RUNNING, newVelocity, trsl, simulationTime };
}

UpdateState simulatePhysAirMovement (const float dt, const float simulationTime, const geom::Vector2D& playerVelocity)
{
    const float gravityAcceleration = 1200.f;
    const float airAcceleration     = 1000.f;
    const float maxFallSpeed        = 600.f;
    const float maxAirMovementSpeed = 600.f;

    const float tickTime = std::max (dt, MIN_TICK_TIME);

    const geom::Vector2D inputVec  = input::getAxisVec();

    const float addedFallVelocity = gravityAcceleration * tickTime;
    const float addedAirMovement  = inputVec.X() * airAcceleration * tickTime;

    float resultAirVelocityX = playerVelocity.X() + addedAirMovement;
    if (playerVelocity.X() * addedAirMovement > 0 && std::abs(resultAirVelocityX) > maxAirMovementSpeed) {
        resultAirVelocityX = playerVelocity.X();
    }
    float resultAirVelocityY = playerVelocity.Y() + addedFallVelocity;
    if (resultAirVelocityY > 0 && std::abs(resultAirVelocityY) > maxFallSpeed) {
        resultAirVelocityY = maxFallSpeed;
    }

    const geom::Vector2D newVelocity (resultAirVelocityX, playerVelocity.Y() + addedFallVelocity);
    debug::log ("Fall velocity: {}", newVelocity);

    const geom::Vector2D trsl = newVelocity * tickTime;

    if (newVelocity.Y() > 0.f) {
        return { Mode::AIR_MOVEMENT, newVelocity, trsl, simulationTime + tickTime };
    }
    else {
        return { Mode::JUMPING, newVelocity, trsl, simulationTime + tickTime };
    }
}

UpdateState simulatePhysJumping (const float dt, float simulationTime, const geom::Vector2D& playerVelocity)
{
    if (simulationTime == 0.f) {
        return simulatePhysAirMovement (dt, simulationTime, geom::Vector2D (playerVelocity.X(), playerVelocity.Y() - 850.f));
    }
    else {
        return simulatePhysAirMovement (dt, simulationTime, playerVelocity);
    }
}

#pragma endregion

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
    const auto           playerState = player.getPhysicalState();
    player::MovementMode targetMode  = selectMovementModeOnAction (playerState.nextAction, playerState.currentMode);
    targetMode                       = updateModeOnConditions (targetMode, player.getCollider(), ground);

    std::vector<player::MovementMode> appliedModes;
    appliedModes.push_back (targetMode);

    float simulationTime = playerState.currentSimTime;
    if (targetMode != playerState.currentMode) {
        simulationTime = 0.f;
    }

    //debug::log ("Action registered {}", playerState.nextAction);
    debug::log ("Targeting movement mode: {}", targetMode);

    switch (targetMode) {
    case Mode::NONE:
    {
        UpdateState physicsState {
            .nextMode = targetMode,
            .velocity = geom::Vector2D(),
            .trsl     = geom::Vector2D(),
            .simTime  = dt,
        };
        return physicsState;
    }
    case Mode::RUNNING:
    {
        return simulatePhys (player, ground, simulatePhysRunning, dt, simulationTime, playerState.velocity);
    }
    case Mode::AIR_MOVEMENT:
    {
        return simulatePhys (player, ground, simulatePhysAirMovement, dt, simulationTime, playerState.velocity);
    }
    case Mode::JUMPING:
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



} // namespace physics::movement
