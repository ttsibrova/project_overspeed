#pragma once
#include <Physics/PhysicsUpdateState.h>
#include <Physics/Point2D.h>

namespace Debug
{

class PlayerMovementDebugger
{
public:
    static PlayerMovementDebugger& GetInstance();

    PlayerMovementDebugger (const PlayerMovementDebugger&) = delete;
    PlayerMovementDebugger operator= (const PlayerMovementDebugger&) = delete;

public:
    void Update (const PhysicsUpdateState& pus, const phs::Point2D& playerPos);
    void Draw();

private:
    PlayerMovementDebugger() {}

private:
    MovementMode m_movementMode {MovementMode::NONE};
    float m_simulationTime {0.f};
    phs::Point2D m_playerPos;
    phs::Vector2D m_playerVelocity;
    phs::Vector2D m_trslVec;
};
}
