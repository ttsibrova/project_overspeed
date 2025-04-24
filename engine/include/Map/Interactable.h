#pragma once
#include <Map/TiledGridPosition.h>
#include <cstdint>
#include <string>
#include <unordered_map>

namespace map {

enum class InteractableTileType : uint16_t
{
    Invalid = 0,

    SliderActive,
    SliderInactive,

    JumpActive,
    JumpInactive
};

struct InteractableTile
{
    InteractableTileType type;
    TiledGridPositon     begin;
    TiledGridPositon     end;
};

InteractableTileType getToggledInteractableTileType (const InteractableTileType source);

} // namespace map
