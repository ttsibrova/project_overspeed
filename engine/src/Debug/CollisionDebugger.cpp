#include <Debug/CollisionDebugger.h>

#include <Debug/DebugDraw.h>
#include <Physics/Collider.h>

namespace Debug {

CollisionDebugger& CollisionDebugger::getInstance()
{
    static CollisionDebugger instance;
    return instance;
}

void CollisionDebugger::update()
{
    m_collisionLines.clear();
    m_colliders.clear();
    m_points.clear();
}

void CollisionDebugger::draw()
{
    for (const auto& [pnt1, pnt2] : m_collisionLines) {
        Debug::draw (pnt1, pnt2);
    }
    for (size_t i = 0, size = m_colliders.size(); i < size; i++) {
        Debug::draw (m_colliders[i], m_colliderColors[i]);
    }
    for (const auto& pnt : m_points) {
        Debug::draw (pnt);
    }
}

void CollisionDebugger::AddCollisionLine (std::pair<geom::Point2D, geom::Point2D> line)
{
    m_collisionLines.push_back (std::move (line));
}

void CollisionDebugger::AddCollider (physics::Collider collider, Color colliderColor)
{
    m_colliders.push_back (std::move (collider));
    m_colliderColors.push_back (std::move (colliderColor));
}

void CollisionDebugger::AddPoint (geom::Point2D pnt)
{
    m_points.push_back (std::move (pnt));
}

} // namespace Debug
