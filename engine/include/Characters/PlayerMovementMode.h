#pragma once
#include <cstdint>

namespace player {

enum class MovementMode: uint8_t
{
    NONE,
    RUNNING,
    JUMPING,
    AIR_MOVEMENT
};

enum class ColliderMode: uint8_t
{
    NOT_SET,
    FULL,
    TIGHT
};

}

