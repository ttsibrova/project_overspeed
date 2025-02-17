#pragma once

#include <Physics/Point2D.h>
#include <Physics/Collider.h>
#include <vector>

namespace Debug
{
class CollisionDebugger
{
public:
    static CollisionDebugger& GetInstance();

    CollisionDebugger (const CollisionDebugger&) = delete;
    CollisionDebugger operator= (const CollisionDebugger&) = delete;

public:
    void Update();
    void Draw();

    void AddCollisionLine (std::pair <phs::Point2D, phs::Point2D> line);
    void AddCollider (phs::Collider collider, Color colliderColor);
    void AddPoint (phs::Point2D pnt);

private:
    CollisionDebugger(){}

private:
    std::vector <std::pair <phs::Point2D, phs::Point2D>> m_collisionLines;
    std::vector <phs::Collider> m_colliders;
    std::vector <Color> m_colliderColors;
    std::vector <phs::Point2D> m_points;
}; 
}
