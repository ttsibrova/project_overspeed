#pragma once
#include <Player/Player.h>
#include <World/Level.h>

namespace interaction {
player::BodyState updateBodyStateOnInteraction (const physics::Collider& interactionPlayerCollider,
                                                player::BodyState playerBodyState, const world::GroundData& ground);

}
