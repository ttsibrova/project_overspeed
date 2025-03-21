#include <Debug/GlobalDebugger.h>

#include <Debug/CollisionDebugger.h>
#include <Debug/PlayerMovementDebugger.h>

namespace Debug {

GlobalDebugger& GlobalDebugger::getInstance()
{
    static GlobalDebugger instance;
    return instance;
}

void GlobalDebugger::update()
{
    CollisionDebugger::getInstance().update();
}

void GlobalDebugger::draw()
{
    CollisionDebugger::getInstance().draw();
    PlayerMovementDebugger::getInstance().draw();
}

} // namespace Debug
