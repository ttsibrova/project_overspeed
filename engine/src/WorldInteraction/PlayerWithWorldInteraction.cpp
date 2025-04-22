#include <WorldInteraction/PlayerWithWorldInteraction.h>

#include <WorldInteraction/GroundCollision.h>

#include <raylib.h>

#include <Debug/DebugLog.h>

#include <ranges>

namespace interaction {

namespace {

std::optional<uint32_t> detectToggledActuator (const physics::Collider&     playerCollider,
                                               const world::LevelActuators& actuators)
{
    for (const auto& [actuatorCollider, actuatorId] : std::views::zip (actuators.getActuatorsColliders(),
                                                                       actuators.getActuatorIds())) {
        if (playerCollider.collides (actuatorCollider)) {
            return actuatorId;
        }
    }
    return std::nullopt;
}

}

player::BodyState updateBodyStateOnInteraction (const physics::Collider& interactionPlayerCollider,
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

player::Action detectActionByInteraction (const physics::Collider&             physicalPlayerCollider,
                                          const world::LevelInteractableTiles& tiles)
{
    for (const auto& [tileType, tileCollider] : tiles.getInteractableTilesTypeWithColliders()) {

        if (physicalPlayerCollider.collides (tileCollider)) {
            switch (tileType) {
            case map::InteractableTileType::JUMP_ACTIVE:
                return player::Action::JUMP;
            case map::InteractableTileType::SLIDER_ACTIVE:
                // return player::Action::SLIDE; // while slide action is not ready
            default:
                continue;
            }
        }
        debug::log (tileCollider, DARKBLUE);
    }
    return player::Action::IDLE;
}

std::optional<uint32_t> detectToggledActuator (const player::Player& player, const world::LevelActuators& actuators)
{
    return detectToggledActuator(player.getCollider(player::ColliderType::INTERACTION), actuators);
}

}


