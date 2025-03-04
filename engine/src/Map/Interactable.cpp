#include <Map/Interactable.h>

namespace map {

InteractableTileType getFlippedActiveState (InteractableTileType source)
{
    switch (source) {
    case InteractableTileType::SLIDER_ACTIVE:
        return InteractableTileType::SLIDER_INACTIVE;
    case InteractableTileType::SLIDER_INACTIVE:
        return InteractableTileType::SLIDER_ACTIVE;
    case InteractableTileType::JUMP_ACTIVE:
        return InteractableTileType::JUMP_INACTIVE;
    case InteractableTileType::JUMP_INACTIVE:
        return InteractableTileType::JUMP_ACTIVE;
    default:
        assert ("false");
        // add new cases
        break;
    }
    std::unreachable();
}

bool isActiveTile (const InteractableTile& tile)
{
    switch (tile.type) {
    case InteractableTileType::SLIDER_ACTIVE:
    case InteractableTileType::JUMP_ACTIVE:
        return true;
    case InteractableTileType::SLIDER_INACTIVE:
    case InteractableTileType::JUMP_INACTIVE:
        return false;
    default:
        assert ("false");
        // add new cases
        break;
    }
    std::unreachable();
}

bool isJumpTile (const InteractableTile& tile)
{
    return tile.type == InteractableTileType::JUMP_ACTIVE 
        || tile.type == InteractableTileType::JUMP_INACTIVE;
}

InteractableTileType recognizeTileType (int tileId)
{
    switch (tileId - 1) {
    case 4:
    case 5:
    case 6:
        return InteractableTileType::SLIDER_ACTIVE;
    case 14:
    case 15:
    case 16:
        return InteractableTileType::SLIDER_INACTIVE;
    case 7:
    case 8:
    case 9:
        return InteractableTileType::JUMP_ACTIVE;
    case 17:
    case 18:
    case 19:
        return InteractableTileType::JUMP_INACTIVE;
    default:
        return InteractableTileType::INCOMPATIBLE;
    };
}

} // namespace map
