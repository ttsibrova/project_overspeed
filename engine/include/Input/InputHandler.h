#pragma once
#include <Input/InputLayer.h>
#include <raylib/raylib.h>

namespace input {

enum class Device
{
    NONE,
    KEYBOARD,
    GAMEPAD
};

}

class InputHandler
{
public:
    static InputHandler& getInstance();
    InputHandler (const InputHandler& other)           = delete;
    InputHandler operator= (const InputHandler& other) = delete;

    template <typename Obj>
    void handleInput (const input::Layer<Obj>& currentLayer, Obj& obj);

    input::Device getActiveDevice() const { return m_lastActiveDevice; }

private:
    InputHandler():
        m_lastActiveDevice (input::Device::KEYBOARD)
    {}

    template <typename Obj>
    bool checkKeyboardInputs (const input::Layer<Obj>& currentLayer, Obj& obj);

    template <typename Obj>
    bool checkGamepadInputs (const input::Layer<Obj>& currentLayer, Obj& obj);

private:
    input::Device m_lastActiveDevice;
};

template <typename Obj>
void InputHandler::handleInput (const input::Layer<Obj>& currentLayer, Obj& obj)
{
    if (m_lastActiveDevice == input::Device::KEYBOARD) {
        if (checkKeyboardInputs (currentLayer, obj)) {
            return;
        }
        if (checkGamepadInputs (currentLayer, obj)) {
            m_lastActiveDevice = input::Device::GAMEPAD;
        }
        return;
    }
    if (m_lastActiveDevice == input::Device::GAMEPAD) {
        if (checkGamepadInputs (currentLayer, obj)) {
            return;
        }
        if (checkKeyboardInputs (currentLayer, obj)) {
            m_lastActiveDevice = input::Device::KEYBOARD;
        }
        return;
    }
    if (m_lastActiveDevice == input::Device::NONE) {
        if (checkKeyboardInputs (currentLayer, obj)) {
            m_lastActiveDevice = input::Device::KEYBOARD;
            return;
        }
        if (checkGamepadInputs (currentLayer, obj)) {
            m_lastActiveDevice = input::Device::GAMEPAD;
        }
        return;
    }
}

template <typename Obj>
bool InputHandler::checkKeyboardInputs (const input::Layer<Obj>& currentLayer, Obj& obj)
{
    for (auto& action : currentLayer.actions) {
        switch (action.type) {
        case input::ActionType::PRESS:
            if (IsKeyPressed (action.mappedKButton)) {
                action.command (obj);
            }
            break;
        case input::ActionType::HOLD:
            if (IsKeyDown (action.mappedKButton)) {
                action.command (obj);
            }
            break;
        }
    }
    return true;
}

template <typename Obj>
bool InputHandler::checkGamepadInputs (const input::Layer<Obj>& currentLayer, Obj& obj)
{
    if (!IsGamepadAvailable (0)) {
        return false;
    }
    for (auto& action : currentLayer.actions) {
        switch (action.type) {
        case input::ActionType::HOLD:
            if (IsGamepadButtonDown (0, action.mappedGButton)) {
                action.command (obj);
                return true;
            }
            break;
        case input::ActionType::PRESS:
            if (IsGamepadButtonPressed (0, action.mappedGButton)) {
                action.command (obj);
                return true;
            }
            break;
        }
    }
    return false;
}
