#include <Debug/DebugLog.h>

#include <Debug/CollisionDebugger.h>
#include <Debug/PlayerMovementDebugger.h>

#include <Physics/Collider.h>
#include <cassert>

namespace Debug {

void Log (const std::pair<geom::Point2D, geom::Point2D>& line, Designation des)
{
    switch (des) {
    case Debug::Collision:
        Debug::CollisionDebugger::GetInstance().AddCollisionLine (line);
        break;
    default:
        assert (false);
        break;
    }
}

void Log (const physics::Collider& collider, const Color& color)
{
    CollisionDebugger::GetInstance().AddCollider (collider, color);
}

void Log (const physics::movement::UpdateState& pus, const geom::Point2D& playerPos)
{
    PlayerMovementDebugger::GetInstance().update (pus, playerPos);
}

void Log (const geom::Point2D& pnt, Designation des)
{
    switch (des) {
    case Debug::Collision:
        Debug::CollisionDebugger::GetInstance().AddPoint (pnt);
        break;
    default:
        assert (false);
        break;
    }
}

} // namespace Debug
