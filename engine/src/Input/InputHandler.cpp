#include <Input/InputHandler.h>

#include <Core/Engine.h>

#include <iostream>

namespace {
static constexpr int gamepadID = 0;
}

namespace input {

void InputHandler::handleInput()
{
    if (!m_layers.empty()) {
        handleTargetedInput (m_layers.back().layer);
    }
}

void InputHandler::handleTargetedInput (const input::Layer& inputLayer)
{
    if (m_lastActiveDevice == input::Device::KEYBOARD) {
        if (checkKeyboardInputs (inputLayer)) {
            return;
        }
        if (checkGamepadInputs (inputLayer)) {
            m_lastActiveDevice = input::Device::GAMEPAD;
        }
        return;
    }
    if (m_lastActiveDevice == input::Device::GAMEPAD) {
        if (checkGamepadInputs (inputLayer)) {
            return;
        }
        if (checkKeyboardInputs (inputLayer)) {
            m_lastActiveDevice = input::Device::KEYBOARD;
        }
        return;
    }
    if (m_lastActiveDevice == input::Device::NONE) {
        if (checkKeyboardInputs (inputLayer)) {
            m_lastActiveDevice = input::Device::KEYBOARD;
            return;
        }
        if (checkGamepadInputs (inputLayer)) {
            m_lastActiveDevice = input::Device::GAMEPAD;
        }
        return;
    }
}

bool InputHandler::checkKeyboardInputs (const input::Layer& inputLayer) const
{
    for (const auto& action : inputLayer.getActions()) {
        switch (action.type) {
        case input::ActionType::PRESS:
            if (IsKeyPressed (action.mappedKButton)) {
                action.command();
            }
            break;
        case input::ActionType::HOLD:
            if (IsKeyDown (action.mappedKButton)) {
                action.command();
            }
            break;
        }
    }
    return true;
}

bool InputHandler::checkGamepadInputs (const input::Layer& inputLayer) const
{
    if (!IsGamepadAvailable (gamepadID)) {
        return false;
    }
    for (const auto& action : inputLayer.getActions()) {
        switch (action.type) {
        case input::ActionType::HOLD:
            if (IsGamepadButtonDown (gamepadID, action.mappedGButton)) {
                action.command();
                return true;
            }
            break;
        case input::ActionType::PRESS:
            if (IsGamepadButtonPressed (gamepadID, action.mappedGButton)) {
                action.command();
                return true;
            }
            break;
        }
    }
    return false;
}

InputHandler& getInputHandler()
{
    return Engine::getInstance().inputHandler;
}

} // namespace input
