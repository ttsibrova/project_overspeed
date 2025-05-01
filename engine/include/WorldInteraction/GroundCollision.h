#pragma once
#include <Physics/Collider.h>
#include <World/Level.h>

#include <optional>

namespace collision {

/*
    std::nullopt means no collision with ground
    translation = Vector2D(0,0) means player is already colliding with ground
*/
std::optional<geom::Vector2D> computeTranslationBeforeCollision (const physics::Collider& playerCollider,
                                                                 const geom::Vector2D&    playerTrsl,
                                                                 const world::GroundData& groundData);

bool isPlayerCollidesWithGround (const physics::Collider& playerCollider, const world::GroundData& groundData);
bool isPlayerGrounded (physics::Collider playerCollider, const world::GroundData& groundData);

} // namespace collision
