#pragma once
#include <cstdint>

namespace player {

enum class CoreState: uint8_t
{
    FULL,
    EMPTY
};

enum class PointerState: uint8_t
{
    SHARP,
    SHIELD
};

struct BodyState {
    CoreState core;
    PointerState pointer;
};

}

