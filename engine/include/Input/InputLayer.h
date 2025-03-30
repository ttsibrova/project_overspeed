#pragma once
#include <functional>
#include <raylib.h>
#include <vector>

namespace input {

enum class ActionType
{
    HOLD,
    PRESS
};

template <typename Obj>
struct Action
{
    GamepadButton              mappedGButton = GAMEPAD_BUTTON_UNKNOWN;
    KeyboardKey                mappedKButton = KEY_NULL;
    ActionType                 type;
    std::function<void (Obj&)> command;
};

template <typename Obj>
struct Layer
{
public:
    void addAction (GamepadButton button, KeyboardKey key, ActionType type, std::function<void (Obj&)> command)
    {
        actions.emplace_back (button, key, type, std::move (command));
    }

public:
    std::vector<Action<Obj>> actions;
};

} // namespace input
