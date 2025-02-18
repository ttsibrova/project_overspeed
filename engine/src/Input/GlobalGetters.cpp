#include <Input/InputGlobalGetters.h>

#include <Geom/Vector2D.h>
#include <Input/InputHandler.h>

namespace input {

geom::Vector2D getAxisVec()
{
    geom::Vector2D res;
    auto device = getActiveDevice();
    switch (device)
    {
    case Device::NONE:
        break;
    case Device::KEYBOARD:
    {
        //if (IsKeyDown (KEY_W) || IsKeyDown (KEY_UP)) {
        //    res.Y() += 1;
        //}
        if (IsKeyDown (KEY_A) || IsKeyDown (KEY_LEFT)) {
            res.X() += -1;
        }
        //if (IsKeyDown (KEY_S) || IsKeyDown (KEY_DOWN)) {
        //    res.Y() += -1;
        //}
        if (IsKeyDown (KEY_D) || IsKeyDown (KEY_RIGHT)) {
            res.X() += 1;
        }
        break;
    }
    case Device::GAMEPAD:
    {
        //if (IsGamepadButtonDown (0, GAMEPAD_BUTTON_LEFT_FACE_UP)) {
        //    res.Y() +=1;
        //}
        if (IsGamepadButtonDown (0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) {
            res.X() += 1;
        }
        //if (IsGamepadButtonDown (0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) {
        //    res.Y() += -1;
        //}
        if (IsGamepadButtonDown (0, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) {
            res.X() += -1;
        }
        break;
    }
    }
    return res;
}

Device getActiveDevice()
{
    return InputHandler::getInstance().getActiveDevice();
}

} // namespace input
