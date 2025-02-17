#include <WorldInteraction/PlayerWithWorldInteraction.h>
#include <World/LevelCollision.h>

player::BodyState interaction::updateBodyStateOnInteraction (const phs::Collider& interactionPlayerCollider,
                                                             player::BodyState playerBodyState,
                                                             const GroundData& ground)
{
    player::BodyState newBodyState;
    if (Collision::IsPlayerCollidesWithGround(interactionPlayerCollider, ground)) {
        newBodyState.pointer = player::PointerState::SHIELD;
    }
    else {
        newBodyState.pointer = player::PointerState::SHARP;
    }
    newBodyState.core = playerBodyState.core;
    return newBodyState;
}
