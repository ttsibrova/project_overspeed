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

struct Action
{
    GamepadButton              mappedGButton = GAMEPAD_BUTTON_UNKNOWN;
    KeyboardKey                mappedKButton = KEY_NULL;
    ActionType                 type;
    std::function<void (void)> command;
};

struct Layer
{
public:
    void addAction (GamepadButton button, KeyboardKey key, ActionType type, std::function<void (void)> command)
    {
        actions.emplace_back (button, key, type, std::move (command));
    }

public:
    std::vector<Action> actions;
};

} // namespace input
