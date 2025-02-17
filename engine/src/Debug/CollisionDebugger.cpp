#include <Debug/CollisionDebugger.h>

#include <Debug/DebugDraw.h>
#include <Physics/Collider.h>

namespace Debug
{
CollisionDebugger& CollisionDebugger::GetInstance()
{
    static CollisionDebugger instance;
    return instance;
}

void CollisionDebugger::Update()
{
    m_collisionLines.clear();
    m_colliders.clear();
    m_points.clear();
}

void CollisionDebugger::Draw()
{
    for (const auto& [pnt1, pnt2]: m_collisionLines) {
        Debug::Draw (pnt1, pnt2);
    }
    for (size_t i = 0, size = m_colliders.size(); i < size; i++) {
        Debug::Draw (m_colliders[i], m_colliderColors[i]);
    }
    for (const auto& pnt: m_points) {
        Debug::Draw (pnt);
    }
}

void CollisionDebugger::AddCollisionLine (std::pair<phs::Point2D, phs::Point2D> line)
{
    m_collisionLines.push_back (std::move (line));
}
void CollisionDebugger::AddCollider (phs::Collider collider, Color colliderColor)
{
    m_colliders.push_back (std::move (collider));
    m_colliderColors.push_back (std::move (colliderColor));
}
void CollisionDebugger::AddPoint (phs::Point2D pnt)
{
    m_points.push_back (std::move (pnt));
}
}

