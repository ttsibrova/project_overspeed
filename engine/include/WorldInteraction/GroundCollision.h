#pragma once
#include <Physics/Collider.h>
#include <World/Level.h>

#include <optional>

namespace collision {

std::optional<geom::Vector2D> hitScanGround (const physics::Collider& playerCollider, const geom::Vector2D& playerTrsl,
                                             const world::GroundData& groundData);

bool isPlayerCollidesWithGround (const physics::Collider& playerCollider, const world::GroundData& groundData);

std::optional<geom::Vector2D> getGroundNormalUnderPlayer (const physics::Collider& playerCollider,
                                                          geom::Quadrant           playerVelocityQuadrant,
                                                          const world::GroundData& groundData);

bool isPlayerGrounded (const physics::Collider& playerCollider, const world::GroundData& groundData);

} // namespace collision
