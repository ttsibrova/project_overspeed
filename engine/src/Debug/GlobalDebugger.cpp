#include <Debug/GlobalDebugger.h>

#include <Debug/CollisionDebugger.h>
#include <Debug/PlayerMovementDebugger.h>


namespace Debug {

GlobalDebugger& GlobalDebugger::GetInstance()
{
    static GlobalDebugger instance;
    return instance;
}
void GlobalDebugger::Update()
{
    CollisionDebugger::GetInstance().Update();
}

void GlobalDebugger::Draw()
{
    CollisionDebugger::GetInstance().Draw();
    PlayerMovementDebugger::GetInstance().Draw();
}

}
