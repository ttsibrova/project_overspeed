#include <WorldInteraction/PlayerWithWorldInteraction.h>

#include <WorldInteraction/GroundCollision.h>

#include <raylib/raylib.h>

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

player::Action interaction::detectActionByInteraction (const physics::Collider&                  physicalPlayerCollider,
                                                       const world::GroundData&                  ground,
                                                       const std::vector<map::InteractableTile>& iTiles)
{
    for (const auto& tile : iTiles) {
        float x_start = static_cast<float> (tile.start.col) * ground.tile_width;
        float y_start = static_cast<float> (tile.start.row) * ground.tile_height;

        float tile_width  = static_cast<float> (tile.end.col - tile.start.col) * ground.tile_width + ground.tile_width;
        float tile_height = static_cast<float> (tile.end.row - tile.start.row) * ground.tile_height + ground.tile_height;

        Rectangle player {
            .x      = physicalPlayerCollider.Min().X() - geom::precision::pixel,
            .y      = physicalPlayerCollider.Min().Y() - geom::precision::pixel,
            .width  = physicalPlayerCollider.Width() + 2 * geom::precision::pixel,
            .height = physicalPlayerCollider.Height() + 2 * geom::precision::pixel,
        };
        Rectangle tileRec {
            .x      = x_start,
            .y      = y_start,
            .width  = tile_width,
            .height = tile_height,
        };

        if (CheckCollisionRecs (player, tileRec)) {
            switch (tile.type) {
            case map::InteractableTileType::JUMP_ACTIVE:
                return player::Action::JUMP;
            case map::InteractableTileType::SLIDER_ACTIVE:
                //return player::Action::SLIDE; // while slide action is not ready
            default:
                continue;
            }
        }

        Debug::Log (physics::Collider (geom::Point2D (x_start, y_start), tile_height, tile_width), DARKBLUE);
    }
    return player::Action::IDLE;
}
