#pragma once
#include <Inputs/Command.h>
#include <Object/IObject.h>
#include <Physics/Vector2D.h>
#include <raylib/raylib.h>
#include <vector>
#include <functional>

enum class Device {
    NONE,
    KEYBOARD,
    GAMEPAD
};

enum class ActionType {
    HOLD,
    PRESS
};

template <typename Obj>
struct InputAction
{
    GamepadButton             m_mappedGButton = GAMEPAD_BUTTON_UNKNOWN;
    KeyboardKey               m_mappedKButton = KEY_NULL;
    ActionType                m_type;
    std::function<void(Obj&)> m_command;
};

template <typename Obj>
struct InputLayer {
public:
    void AddAction (GamepadButton button, KeyboardKey key, ActionType type, std::function <void(Obj&)> command)
    {
        m_layerActions.emplace_back (button, key, type, std::move (command));
    }

public:
    std::vector <InputAction<Obj>> m_layerActions;
};


class InputHandler
{
public:

    static InputHandler& GlobalInstance();
    InputHandler (const InputHandler& other) = delete;
    InputHandler operator= (const InputHandler& other) = delete;

    template <typename Obj>
    void HandleInput (const InputLayer <Obj>& currentLayer, Obj& obj)
    {
        if (m_lastActiveDevice == Device::KEYBOARD) {
            if (CheckKeyboardInputs (currentLayer, obj)) {
                return;
            }
            if (CheckGamepadInputs (currentLayer, obj)) {
                m_lastActiveDevice = Device::GAMEPAD;
            }
            return;
        }
        if (m_lastActiveDevice == Device::GAMEPAD) {
            if (CheckGamepadInputs (currentLayer, obj)) {
                return;
            }
            if (CheckKeyboardInputs (currentLayer, obj)) {
                m_lastActiveDevice = Device::KEYBOARD;
            }
            return;
        }
        if (m_lastActiveDevice == Device::NONE) {
            if (CheckKeyboardInputs (currentLayer, obj)) {
                m_lastActiveDevice = Device::KEYBOARD;
                return;
            }
            if (CheckGamepadInputs (currentLayer, obj)) {
                m_lastActiveDevice = Device::GAMEPAD;
            }
            return;
        }
    }


    Device GetActiveDevice() const { return m_lastActiveDevice; }

private:
    InputHandler ():
        m_lastActiveDevice (Device::KEYBOARD)
    {}

    template <typename Obj>
    bool CheckKeyboardInputs (const InputLayer <Obj>& currentLayer, Obj& obj)
    {
        for (auto& action : currentLayer.m_layerActions) {
            switch (action.m_type)
            {
            case ActionType::HOLD:
                if (IsKeyDown (action.m_mappedKButton)) {
                    action.m_command(obj);
                    return true;
                }
                break;
            case ActionType::PRESS:
                if (IsKeyPressed (action.m_mappedKButton)) {
                    action.m_command (obj);
                    return true;
                }
                break;
            }
        }
        return false;
    }

    template <typename Obj>
    bool CheckGamepadInputs (const InputLayer <Obj>& currentLayer, Obj& obj)
    {
        if (!IsGamepadAvailable (0)) {
            return false;
        }
        for (auto& action : currentLayer.m_layerActions) {
            switch (action.m_type)
            {
            case ActionType::HOLD:
                if (IsGamepadButtonDown (0, action.m_mappedGButton)) {
                    action.m_command (obj);
                    return true;
                }
                break;
            case ActionType::PRESS:
                if (IsGamepadButtonPressed (0, action.m_mappedGButton)) {
                    action.m_command (obj);
                    return true;
                }
                break;
            }
        }
        return false;
    }


private:
    Device m_lastActiveDevice;
};


namespace Input
{
    phs::Vector2D GetAxisVec();
    inline Device GetActiveDevice() { return InputHandler::GlobalInstance().GetActiveDevice(); }
}

