#include <Debug/DebugDraw.h>
#include <Debug/PlayerMovementDebugger.h>

namespace debug {

PlayerMovementDebugger& PlayerMovementDebugger::getInstance()
{
    static PlayerMovementDebugger instance;
    return instance;
}
void PlayerMovementDebugger::update (const physics::movement::UpdateState& pmUpdateState, const geom::Point2D& playerPos)
{
    m_playerPos      = playerPos;
    m_movementMode   = pmUpdateState.nextMode;
    m_playerVelocity = pmUpdateState.velocity;
    m_simulationTime = pmUpdateState.simTime;
    m_trslVec        = pmUpdateState.trsl;
}

void PlayerMovementDebugger::draw() const
{
    debug::draw (m_playerPos, m_playerVelocity);
    debug::draw (geom::Point2D (m_playerPos.X(), m_playerPos.Y() - 30), m_trslVec, RED);
}

} // namespace debug
