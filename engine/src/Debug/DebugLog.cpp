#include <Debug/DebugLog.h>

#include <Debug/CollisionDebugger.h>
#include <Debug/PlayerMovementDebugger.h>

#include <Physics/Collider.h>
#include <cassert>


namespace Debug {
void Log (const std::pair<phs::Point2D, phs::Point2D>& line, Designation des)
{
    switch (des)
    {
    case Debug::Collision:
        Debug::CollisionDebugger::GetInstance().AddCollisionLine (line);
        break;
    default:
        assert (false);
        break;
    }
}

void Log (const Collider& collider)
{
    CollisionDebugger::GetInstance().AddCollider (collider);
}

void Log (const PhysicsUpdateState& pus, const phs::Point2D& playerPos)
{
    PlayerMovementDebugger::GetInstance().Update (pus, playerPos);
}

void Log (const phs::Point2D& pnt, Designation des)
{
    switch (des)
    {
    case Debug::Collision:
        Debug::CollisionDebugger::GetInstance().AddPoint (pnt);
        break;
    default:
        assert (false);
        break;
    }
}


}
