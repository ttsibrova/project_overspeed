#include <Debug/DebugLog.h>

#include <Core/Engine.h>
#include <Core/Logger.h>

#include <Geom/LineSegment.h>

#include <Debug/CollisionDebugger.h>
#include <Debug/PlayerMovementDebugger.h>

#include <Physics/Collider.h>
#include <cassert>

namespace debug {

void log (const geom::LineSegment& segment, Designation des)
{
    switch (des) {
    case debug::Collision:
        debug::CollisionDebugger::getInstance().addCollisionSegment (segment);
        break;
    default:
        assert (false);
        break;
    }
}

void log (const physics::Collider& collider, const Color& color)
{
    CollisionDebugger::getInstance().addCollider (collider, color);
}

void log (const physics::movement::UpdateState& pus, const geom::Point2D& playerPos)
{
    PlayerMovementDebugger::getInstance().update (pus, playerPos);
}

void log (const geom::Point2D& pnt, Designation des)
{
    switch (des) {
    case debug::Collision:
        debug::CollisionDebugger::getInstance().addPoint (pnt);
        break;
    default:
        assert (false);
        break;
    }
}

void log (const std::string& message)
{
    Engine::getInstance().logger.debug (message);
}

} // namespace debug
