#include <Inputs/InputHandler.h>
#include <iostream>

static constexpr int gamepadID = 0;



InputHandler& InputHandler::GlobalInstance()
{
    static InputHandler iHandler;
    return iHandler;
}




phs::Vector2D Input::GetAxisVec()
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

