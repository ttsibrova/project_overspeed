#pragma once
#include <Geom/Point2D.h>
#include <Physics/PhysicsUpdateState.h>

namespace Debug
{

class PlayerMovementDebugger
{
public:
    static PlayerMovementDebugger& GetInstance();

    PlayerMovementDebugger (const PlayerMovementDebugger&) = delete;
    PlayerMovementDebugger operator= (const PlayerMovementDebugger&) = delete;

public:
    void update (const physics::movement::UpdateState& pmUpdateState, const geom::Point2D& playerPos);
    void draw();

private:
    PlayerMovementDebugger() {}

private:
    player::MovementMode m_movementMode {player::MovementMode::NONE};
    float m_simulationTime {0.f};
    geom::Point2D m_playerPos;
    geom::Vector2D m_playerVelocity;
    geom::Vector2D m_trslVec;
};
}
