#include <WorldInteraction/PlayerWithWorldInteraction.h>

#include <WorldInteraction/GroundCollision.h>

player::BodyState interaction::updateBodyStateOnInteraction (const physics::Collider& interactionPlayerCollider,
                                                             player::BodyState playerBodyState, const world::GroundData& ground)
{
    player::BodyState newBodyState;
    if (Collision::IsPlayerCollidesWithGround (interactionPlayerCollider, ground)) {
        newBodyState.pointer = player::PointerState::SHIELD;
    }
    else {
        newBodyState.pointer = player::PointerState::SHARP;
    }
    newBodyState.core = playerBodyState.core;
    return newBodyState;
}
