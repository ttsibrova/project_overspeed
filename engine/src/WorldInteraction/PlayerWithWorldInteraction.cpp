#include <WorldInteraction/PlayerWithWorldInteraction.h>

#include <WorldInteraction/GroundCollision.h>

#include <raylib.h>

#include <Debug/DebugLog.h>

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

player::Action interaction::detectActionByInteraction (const physics::Collider&             physicalPlayerCollider,
                                                       const world::LevelInteractableTiles& tiles)
{
    for (const auto& [tileType, tileCollider]: tiles.getInteractableTilesTypeWithColliders()) {

        if (physicalPlayerCollider.collides (tileCollider)) {
            switch (tileType) {
            case map::InteractableTileType::JUMP_ACTIVE:
                return player::Action::JUMP;
            case map::InteractableTileType::SLIDER_ACTIVE:
                //return player::Action::SLIDE; // while slide action is not ready
            default:
                continue;
            }
        }
        Debug::Log (tileCollider, DARKBLUE);
    }
    return player::Action::IDLE;
}
