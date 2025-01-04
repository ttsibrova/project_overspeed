#pragma once
#include <cstdint>

enum class PlayerAction: uint8_t
{
    IDLE,
    MOVE,
    SLIDE,
    JUMP,
};
