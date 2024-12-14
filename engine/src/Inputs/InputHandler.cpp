#include <Inputs/InputHandler.h>
#include <iostream>

bool InputHandler::CheckKeyboardInputs()
{
    for (auto& action : m_inputLayers.top()->m_layerActions) {
        switch (action.m_type)
        {
        case ActionType::HOLD:
            if (IsKeyDown (action.m_mappedKButton)) {
                action.m_command->Execute (m_objectsStack.top());
                return true;
            }
            break;
        case ActionType::PRESS:
            if (IsKeyPressed (action.m_mappedKButton)) {
                action.m_command->Execute (m_objectsStack.top());
                return true;
            }
            break;
        }
    }
    return false;
}

bool InputHandler::CheckGamepadInputs()
{
    if (!IsGamepadAvailable (m_gamepadId)) {
        return false;
    }
    for (auto& action : m_inputLayers.top()->m_layerActions) {
        switch (action.m_type)
        {
        case ActionType::HOLD:
            if (IsGamepadButtonDown (m_gamepadId, action.m_mappedGButton)) {
                action.m_command->Execute (m_objectsStack.top());
                return true;
            }
            break;
        case ActionType::PRESS:
            if (IsGamepadButtonPressed (m_gamepadId, action.m_mappedGButton)) {
                action.m_command->Execute (m_objectsStack.top());
                return true;
            }
            break;
        }
    }
    return false;
}


InputHandler& InputHandler::GlobalInstance()
{
    static InputHandler iHandler;
    return iHandler;
}

void InputHandler::HandleInput()
{
    if (!m_inputLayers.empty() && !m_objectsStack.empty()) {

        if (m_lastActiveDevice == Device::KEYBOARD) {
            if (CheckKeyboardInputs()) {
                return;
            }
            if (CheckGamepadInputs()) {
                m_lastActiveDevice = Device::GAMEPAD;
            }
            return;
        }
        if (m_lastActiveDevice == Device::GAMEPAD) {
            if (CheckGamepadInputs()) {
                return;
            }
            if (CheckKeyboardInputs()) {
                m_lastActiveDevice = Device::KEYBOARD;
            }
            return;
        }
        if (m_lastActiveDevice == Device::NONE) {
            if (CheckKeyboardInputs()) {
                m_lastActiveDevice = Device::KEYBOARD;
                return;
            }
            if (CheckGamepadInputs()) {
                m_lastActiveDevice = Device::GAMEPAD;
            }
            return;
        }
    }
}

void InputHandler::PushInputLayer (InputLayer* newLayer)
{
    m_inputLayers.push (newLayer);
}

bool InputHandler::PopInputLayer()
{
    if (m_inputLayers.empty()) 
        return false;

    m_inputLayers.pop();
    return true;
}

void InputHandler::PushObject (IObject* obj)
{
    m_objectsStack.push (obj);
}

bool InputHandler::PopObject()
{
    if (m_objectsStack.empty())
        return false;

    m_objectsStack.pop();
    return true;
}

phs::Vector2D InputHandler::GetAxisVec()
{
    phs::Vector2D res;
    auto device = GetActiveDevice();
    switch (device)
    {
    case Device::NONE:
        break;
    case Device::KEYBOARD:
    {
        if (IsKeyDown (KEY_W) || IsKeyDown (KEY_UP)) {
            res.Y() += 1;
        }
        if (IsKeyDown (KEY_A) || IsKeyDown (KEY_LEFT)) {
            res.X() += -1;
        }
        if (IsKeyDown (KEY_S) || IsKeyDown (KEY_DOWN)) {
            res.Y() += -1;
        }
        if (IsKeyDown (KEY_D) || IsKeyDown (KEY_RIGHT)) {
            res.X() += 1;
        }
        break;
    }
    case Device::GAMEPAD:
    {
        if (IsGamepadButtonDown (0, GAMEPAD_BUTTON_LEFT_FACE_UP)) {
            res.Y() +=1;
        }
        if (IsGamepadButtonDown (0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) {
            res.X() += 1;
        }
        if (IsGamepadButtonDown (0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) {
            res.Y() += -1;
        }
        if (IsGamepadButtonDown (0, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) {
            res.X() += -1;
        }
        break;
    }
    }
    return res;
}

