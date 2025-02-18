#pragma once

#include <Geom/Point2D.h>
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
    void update();
    void draw();

    void AddCollisionLine (std::pair <geom::Point2D, geom::Point2D> line);
    void AddCollider (physics::Collider collider, Color colliderColor);
    void AddPoint (geom::Point2D pnt);

private:
    CollisionDebugger(){}

private:
    std::vector <std::pair <geom::Point2D, geom::Point2D>> m_collisionLines;
    std::vector <physics::Collider> m_colliders;
    std::vector <Color> m_colliderColors;
    std::vector <geom::Point2D> m_points;
}; 
}
