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
    case Mode::None:
        switch (action) {
        case player::Action::Idle : return Mode::None;
        case player::Action::Move : return Mode::Moving;
        case player::Action::Slide: return Mode::None;    // temporary
        case player::Action::Jump : return Mode::Jumping;
        default   : assert (false); return Mode::None;
        };
    case Mode::Moving:
        switch (action) {
        case player::Action::Idle : return Mode::None;
        case player::Action::Move : return Mode::Moving;
        case player::Action::Slide: return Mode::None;  // temporary
        case player::Action::Jump : return Mode::Jumping;
        default   : assert (false); return Mode::None;
        }
    case Mode::Jumping: //
        return Mode::Jumping; // disallows switching to other modes util simulation is done
    case Mode::AirMoving: //
        return Mode::AirMoving; // disallows switching to other modes util simulation is done
    default:
        assert (false); // should not be there
        return Mode::None;
    }
}

player::MovementMode updateModeOnConditions (player::MovementMode     targetMode,
                                             const physics::Collider& playerCollider,
                                             const world::GroundData& ground)
{
    bool isGrounded = collision::isPlayerGrounded (playerCollider, ground);
    switch (targetMode) {
    case Mode::None:
    case Mode::Moving:
        if (!isGrounded) {
            return Mode::AirMoving;
        }
        break;
    case Mode::Jumping: break;
    case Mode::AirMoving:
        if (isGrounded) {
            return Mode::None;
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

    //__assume (simulationTime >= 0.f);
    //__assume (dt >= 0.f);

    const auto inputVec = geom::stripByAxis (input::getAxisVec(), geom::Axis::x);
    if (inputVec.getSquareMagnitude() < 1e-5 || std::abs (inputVec.x) < 1e-5) {
        return { Mode::None, playerVelocity, geom::Vector2D(), 0. }; // no direction
    }

    geom::Vector2D newVelocity         = geom::stripByAxis (playerVelocity, geom::Axis::x);
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

    return { Mode::Moving, newVelocity, trsl, simulationTime };
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
    const float addedAirMovement  = inputVec.x * airAcceleration * tickTime;

    float resultAirVelocityX = playerVelocity.x + addedAirMovement;
    if (playerVelocity.x * addedAirMovement > 0 && std::abs(resultAirVelocityX) > maxAirMovementSpeed) {
        resultAirVelocityX = playerVelocity.x;
    }
    float resultAirVelocityY = playerVelocity.y + addedFallVelocity;
    if (resultAirVelocityY > 0 && std::abs(resultAirVelocityY) > maxFallSpeed) {
        resultAirVelocityY = maxFallSpeed;
    }

    const geom::Vector2D newVelocity (resultAirVelocityX, playerVelocity.y + addedFallVelocity);
    debug::log ("Fall velocity: {}", newVelocity);

    const geom::Vector2D trsl = newVelocity * tickTime;

    if (newVelocity.y > 0.f) {
        return { Mode::AirMoving, newVelocity, trsl, simulationTime + tickTime };
    }
    else {
        return { Mode::Jumping, newVelocity, trsl, simulationTime + tickTime };
    }
}

UpdateState simulatePhysJumping (const float dt, float simulationTime, const geom::Vector2D& playerVelocity)
{
    if (simulationTime == 0.f) {
        return simulatePhysAirMovement (dt, simulationTime, geom::Vector2D (playerVelocity.x, playerVelocity.y - 850.f));
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
    auto        adjustedVec    = collision::computeTranslationBeforeCollision (playerCollider, physUpdate.trsl, ground);
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
    case Mode::None:
        return UpdateState { .nextMode = targetMode, .velocity = geom::Vector2D(), .trsl = geom::Vector2D(), .simTime = dt };
    case Mode::Moving:
        return simulatePhys (player, ground, simulatePhysRunning, dt, simulationTime, playerState.velocity);
    case Mode::AirMoving:
        return simulatePhys (player, ground, simulatePhysAirMovement, dt, simulationTime, playerState.velocity);
    case Mode::Jumping:
        return simulatePhys (player, ground, simulatePhysJumping, dt, simulationTime, playerState.velocity);
    }
    
    float newSimTime = dt;
    if (targetMode == playerState.currentMode) {
        newSimTime += playerState.currentSimTime;
    }
    UpdateState emptyPhysicsUpdate { targetMode, playerState.velocity, geom::Vector2D(), newSimTime };
    return emptyPhysicsUpdate;
}



} // namespace physics::movement
