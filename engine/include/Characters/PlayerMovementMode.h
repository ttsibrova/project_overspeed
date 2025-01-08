#pragma once
#include <cstdint>

enum class MovementMode: uint8_t
{
    NONE,
    RUNNING,
    JUMPING,
    FALLING
};

