#pragma once
#include <Physics/Collider.h>
#include <World/Level.h>

#include <optional>

namespace Collision {

std::optional<geom::Vector2D> HitScanGround (const physics::Collider& playerCollider, const geom::Vector2D& playerTrsl,
                                             const world::GroundData& groundData);

bool IsPlayerCollidesWithGround (const physics::Collider& playerCollider, const world::GroundData& groundData);

std::optional<geom::Vector2D> GetGroundNormalUnderPlayer (const physics::Collider& playerCollider,
                                                          geom::Quadrant           playerVelocityQuadrant,
                                                          const world::GroundData& groundData);

bool IsPlayerGrounded (const physics::Collider& playerCollider, const world::GroundData& groundData);

} // namespace Collision
