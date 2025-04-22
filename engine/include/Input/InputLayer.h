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
    GamepadButton         mappedGButton = GAMEPAD_BUTTON_UNKNOWN;
    KeyboardKey           mappedKButton = KEY_NULL;
    ActionType            type;
    std::function<void()> command;
};

struct Layer
{
    void addAction (GamepadButton button, KeyboardKey key, ActionType type, std::function<void()> command)
    {
        actions.emplace_back (button, key, type, std::move (command));
    }

    const std::vector<Action>& getActions() const { return actions; }

private:
    std::vector<Action> actions;
};

} // namespace input
