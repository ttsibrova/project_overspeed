#pragma once
#include <cstdint>

namespace player {

enum class CoreState : uint8_t
{
    Full,
    Empty
};

enum class PointerState : uint8_t
{
    Sharp,
    Shield
};

struct BodyState
{
    CoreState    core;
    PointerState pointer;
};

} // namespace player
