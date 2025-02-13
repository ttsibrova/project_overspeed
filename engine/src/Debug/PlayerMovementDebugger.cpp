#include <Debug/PlayerMovementDebugger.h>
#include <Debug/DebugDraw.h>

namespace Debug
{
PlayerMovementDebugger& PlayerMovementDebugger::GetInstance()
{
    static PlayerMovementDebugger instance;
    return instance;
}
void PlayerMovementDebugger::Update (const PhysicsUpdateState& pus, const phs::Point2D& playerPos)
{
    m_playerPos = playerPos;
    m_movementMode = pus.nextMode;
    m_playerVelocity = pus.velocity;
    m_simulationTime = pus.simTime;
    m_trslVec = pus.trsl;
}
void PlayerMovementDebugger::Draw()
{
    Debug::Draw (m_playerPos, m_playerVelocity);
    Debug::Draw (phs::Point2D (m_playerPos.X(), m_playerPos.Y() - 30), m_trslVec, RED);
}
}
