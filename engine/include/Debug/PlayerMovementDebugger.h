#pragma once
#include <Geom/Point2D.h>
#include <Physics/PhysicsUpdateState.h>

namespace debug {

class PlayerMovementDebugger
{
public:
    static PlayerMovementDebugger& getInstance();

    PlayerMovementDebugger (const PlayerMovementDebugger&)           = delete;
    PlayerMovementDebugger operator= (const PlayerMovementDebugger&) = delete;

public:
    void update (const physics::movement::UpdateState& pmUpdateState, const geom::Point2D& playerPos);
    void draw() const;

private:
    PlayerMovementDebugger() {}

private:
    player::MovementMode m_movementMode   = { player::MovementMode::None };
    float                m_simulationTime = { 0.f };
    geom::Point2D        m_playerPos      = {};
    geom::Vector2D       m_playerVelocity;
    geom::Vector2D       m_trslVec;
};

} // namespace debug
