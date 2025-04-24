#include <Input/InputGlobalGetters.h>

#include <Geom/Vector2D.h>
#include <Input/InputHandler.h>

namespace input {

geom::Vector2D getAxisVec()
{
    // So far it seems that it makes sense to implement this function properly
    // until I have a proper mapping settings for controls available globally

    geom::Vector2D res;
    const Device   device = getActiveDevice();
    switch (device) {
    case Device::None:
        break;
    case Device::Keyboard:
    {
        if (IsKeyDown (KEY_A) || IsKeyDown (KEY_LEFT)) {
            res.x += -1;
        }
        if (IsKeyDown (KEY_D) || IsKeyDown (KEY_RIGHT)) {
            res.x += 1;
        }
        break;
    }
    case Device::Gamepad:
    {
        if (IsGamepadButtonDown (0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) {
            res.x += 1;
        }
        if (IsGamepadButtonDown (0, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) {
            res.x += -1;
        }
        break;
    }
    }
    return res;
}

Device getActiveDevice()
{
    return input::getInputHandler().getActiveDevice();
}

} // namespace input
