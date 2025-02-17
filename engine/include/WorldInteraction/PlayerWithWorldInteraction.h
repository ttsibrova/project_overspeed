#pragma once
#include <Characters/Player.h>

namespace interaction
{

player::BodyState updateBodyStateOnInteraction (const phs::Collider& interactionPlayerCollider, player::BodyState playerBodyState, const GroundData& ground);

}
