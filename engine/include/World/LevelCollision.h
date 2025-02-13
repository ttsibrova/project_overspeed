#pragma once
#include <Physics/Collider.h>
#include <World/Level.h>

#include <optional>

namespace Collision
{
std::optional <phs::Vector2D> HitScanGround (const phs::Collider& playerCollider,
                                             const phs::Vector2D& playerTrsl,
                                             const GroundData& groundData);

bool IsPlayerCollidesWithGround (const phs::Collider& playerCollider,
                                 const GroundData& groundData);

std::optional <phs::Vector2D> GetGroundNormalUnderPlayer (const phs::Collider& playerCollider,
                                                          phs::Quadrant playerVelocityQuadrant,
                                                          const GroundData& groundData);

bool IsPlayerGrounded (const phs::Collider& playerCollider, const GroundData& groundData);

}
