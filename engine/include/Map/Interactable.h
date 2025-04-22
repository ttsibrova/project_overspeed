#pragma once
#include <Map/TiledGridPosition.h>
#include <cstdint>
#include <string>
#include <unordered_map>

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
    TiledGridPositon     begin;
    TiledGridPositon     end;
};

InteractableTileType getToggledInteractableTileType (const InteractableTileType source);

} // namespace map
