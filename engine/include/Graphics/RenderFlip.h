#pragma once
#include <cstdint>

namespace graphics {

enum RenderFlip : uint8_t
{
    FlipNone       = 0x00000000,
    FlipHorizontal = 0x00000001,
    FlipVertical   = 0x00000002
};

}
