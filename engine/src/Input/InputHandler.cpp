#include <Input/InputHandler.h>
#include <iostream>

static constexpr int gamepadID = 0;

InputHandler& InputHandler::getInstance()
{
    static InputHandler iHandler;
    return iHandler;
}

