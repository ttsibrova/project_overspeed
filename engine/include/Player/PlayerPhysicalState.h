#pragma once
#include <Geom/Vector2D.h>
#include <cstdint>

namespace player {

enum class Action : uint8_t
{
    Idle,
    Move,
    Slide,
    Jump,
};

enum class MovementMode : uint8_t
{
    None,
    Moving,
    Jumping,
    AirMoving
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
        case player::Action::Idle : actionName = "Idle"; break;
        case player::Action::Move : actionName = "Move"; break;
        case player::Action::Slide: actionName = "Slide"; break;
        case player::Action::Jump : actionName = "Jump"; break;
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
        case player::MovementMode::None: modeName = "None"; break;
        case player::MovementMode::Moving: modeName = "Moving"; break;
        case player::MovementMode::Jumping: modeName = "Jumping"; break;
        case player::MovementMode::AirMoving: modeName = "AirMoving"; break;
        }
        return std::format_to (ctx.out(), "{}", modeName);
    }
};
