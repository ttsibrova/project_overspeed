#pragma once
#include <Player/Player.h>
#include <World/Level.h>
#include <World/LevelInteractableTiles.h>

namespace interaction {
player::BodyState updateBodyStateOnInteraction (const physics::Collider& interactionPlayerCollider,
                                                player::BodyState playerBodyState, const world::GroundData& ground);

player::Action detectActionByInteraction (const physics::Collider& physicalPlayerCollider, const world::LevelInteractableTiles& tiles);
}
