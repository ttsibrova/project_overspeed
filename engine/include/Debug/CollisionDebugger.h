#pragma once

#include <Geom/LineSegment.h>
#include <Geom/Point2D.h>
#include <Physics/Collider.h>

#include <vector>

namespace debug {
class CollisionDebugger
{
public:
    static CollisionDebugger& getInstance();

    CollisionDebugger (const CollisionDebugger&)           = delete;
    CollisionDebugger operator= (const CollisionDebugger&) = delete;

public:
    void update();
    void draw() const;

    void addCollisionSegment (geom::LineSegment segment);
    void addCollider (physics::Collider collider, Color colliderColor);
    void addPoint (geom::Point2D pnt);

private:
    CollisionDebugger() {}

private:
    std::vector<geom::LineSegment> m_collisionSegments;
    std::vector<physics::Collider> m_colliders;
    std::vector<Color>             m_colliderColors;
    std::vector<geom::Point2D>     m_points;
};
} // namespace debug
