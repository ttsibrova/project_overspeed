#include <Debug/GlobalDebugger.h>

#include <Debug/CollisionDebugger.h>
#include <Debug/PlayerMovementDebugger.h>

namespace Debug {

GlobalDebugger& GlobalDebugger::GetInstance()
{
    static GlobalDebugger instance;
    return instance;
}

void GlobalDebugger::update()
{
    CollisionDebugger::GetInstance().update();
}

void GlobalDebugger::draw()
{
    CollisionDebugger::GetInstance().draw();
    PlayerMovementDebugger::GetInstance().draw();
}

} // namespace Debug
