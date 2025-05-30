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
    for (const auto& [actuatorCollider, actuatorId] : std::views::zip (actuators.getColliders(),
                                                                       actuators.getIds())) {
        if (playerCollider.collides (actuatorCollider)) {
            return actuatorId;
        }
    }
    return std::nullopt;
}

} // namespace

player::BodyState updateBodyStateOnInteraction (const physics::Collider& interactionPlayerCollider,
                                                player::BodyState        playerBodyState,
                                                const world::GroundData& ground)
{
    player::BodyState newBodyState;
    if (collision::isPlayerCollidesWithGround (interactionPlayerCollider, ground)) {
        newBodyState.pointer = player::PointerState::Shield;
    }
    else {
        newBodyState.pointer = player::PointerState::Sharp;
    }
    newBodyState.core = playerBodyState.core;
    return newBodyState;
}

player::Action detectActionByInteraction (const physics::Collider&             physicalPlayerCollider,
                                          const world::LevelInteractableTiles& tiles)
{
    for (const auto& [tileType, tileCollider] : std::views::zip (tiles.getTypes(), tiles.getColliders())) {

        if (physicalPlayerCollider.collides (tileCollider)) {
            switch (tileType) {
            case map::InteractableTileType::JumpActive:
                return player::Action::Jump;
            case map::InteractableTileType::SliderActive:
                // return player::Action::SLIDE; // while slide action is not ready
            default:
                continue;
            }
        }
        debug::log (tileCollider, DARKBLUE);
    }
    return player::Action::Idle;
}

std::optional<uint32_t> detectToggledActuator (const player::Player& player, const world::LevelActuators& actuators)
{
    return detectToggledActuator(player.getCollider(player::ColliderType::INTERACTION), actuators);
}

}


