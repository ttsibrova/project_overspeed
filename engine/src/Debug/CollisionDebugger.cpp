#include <Debug/CollisionDebugger.h>

#include <Debug/DebugDraw.h>
#include <Physics/Collider.h>

namespace debug {

CollisionDebugger& CollisionDebugger::getInstance()
{
    static CollisionDebugger instance;
    return instance;
}

void CollisionDebugger::update()
{
    m_collisionSegments.clear();
    m_colliders.clear();
    m_points.clear();
}

void CollisionDebugger::draw() const
{
    for (const auto& [pnt1, pnt2] : m_collisionSegments) {
        debug::draw (pnt1, pnt2);
    }
    for (size_t i = 0, size = m_colliders.size(); i < size; i++) {
        debug::draw (m_colliders[i], m_colliderColors[i]);
    }
    for (const auto& pnt : m_points) {
        debug::draw (pnt);
    }
}

void CollisionDebugger::addCollisionSegment (geom::LineSegment segment)
{
    m_collisionSegments.push_back (std::move (segment));
}

void CollisionDebugger::addCollider (physics::Collider collider, Color colliderColor)
{
    m_colliders.push_back (std::move (collider));
    m_colliderColors.push_back (std::move (colliderColor));
}

void CollisionDebugger::addPoint (geom::Point2D pnt)
{
    m_points.push_back (std::move (pnt));
}

} // namespace debug
