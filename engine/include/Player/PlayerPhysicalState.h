#pragma once
#include <Geom/Vector2D.h>
#include <cstdint>

namespace player {

enum class Action : uint8_t
{
    IDLE,
    MOVE,
    SLIDE,
    JUMP,
};

enum class MovementMode : uint8_t
{
    NONE,
    RUNNING,
    JUMPING,
    AIR_MOVEMENT
};

struct PhysicalState
{
    Action         nextAction;
    MovementMode   currentMode;
    float          currentSimTime;
    geom::Vector2D velocity;
};

} // namespace player

template <>
struct std::formatter<player::Action>
{
    constexpr auto parse (std::format_parse_context& ctx) { return ctx.begin(); }

    auto format (const player::Action& a, std::format_context& ctx) const
    {
        std::string actionName = "<None>";
        switch (a) {
        case player::Action::IDLE : actionName = "IDLE"; break;
        case player::Action::MOVE : actionName = "MOVE"; break;
        case player::Action::SLIDE: actionName = "SLIDE"; break;
        case player::Action::JUMP : actionName = "JUMP"; break;
        }
        return std::format_to (ctx.out(), "{}", actionName);
    }
};

template <>
struct std::formatter<player::MovementMode>
{
    constexpr auto parse (std::format_parse_context& ctx) { return ctx.begin(); }

    auto format (const player::MovementMode& m, std::format_context& ctx) const
    {
        std::string modeName = "<None>";
        switch (m) {
        case player::MovementMode::NONE: modeName = "NONE"; break;
        case player::MovementMode::RUNNING: modeName = "RUNNING"; break;
        case player::MovementMode::JUMPING: modeName = "JUMPING"; break;
        case player::MovementMode::AIR_MOVEMENT: modeName = "AIR_MOVEMENT"; break;
        }
        return std::format_to (ctx.out(), "{}", modeName);
    }
};
