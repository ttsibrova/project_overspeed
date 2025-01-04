#pragma once
#include <Physics/Collider.h>
#include <World/Level.h>

#include <optional>

namespace Collision
{

std::optional <phs::Vector2D> ComputeCollisionWithGround (const Collider& playerCollider, const phs::Vector2D& playerTrsl, const GroundData& groundData);
bool IsPlayerGrounded (const Collider& playerCollider, const GroundData& groundData);

//void AdjustTranslation ();
}
