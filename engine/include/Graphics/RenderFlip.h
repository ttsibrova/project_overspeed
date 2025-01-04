#pragma once
#include <cstdint>

enum RenderFlip: uint8_t
{
    FLIP_NONE = 0x00000000,
    FLIP_HORIZONTAL = 0x00000001,
    FLIP_VERTICAL = 0x00000002
};
