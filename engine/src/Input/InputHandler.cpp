#include <Input/InputHandler.h>
#include <iostream>

namespace {
static constexpr int gamepadID = 0;
}

InputHandler& InputHandler::getInstance()
{
    static InputHandler iHandler;
    return iHandler;
}
