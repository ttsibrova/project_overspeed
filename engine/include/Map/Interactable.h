#pragma once
#include <Map/Tileset.h>
#include <cassert>
#include <cstdint>

namespace map {

enum class InteractableTileType : uint16_t
{
    INCOMPATIBLE = 0,

    SLIDER_ACTIVE,
    SLIDER_INACTIVE,

    JUMP_ACTIVE,
    JUMP_INACTIVE
};

struct InteractableTile
{
    InteractableTileType type;
    TilePos              start;
    TilePos              end;
};

InteractableTileType getFlippedActiveState (InteractableTileType source);
InteractableTileType recognizeTileType (int tileId);

bool isActiveTile (const InteractableTile& tile);
bool isJumpTile (const InteractableTile& tile);

} // namespace map
