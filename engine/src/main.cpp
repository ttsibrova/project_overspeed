#include <Core/Engine.h>
#include <raylib/raylib.h>

int main() {

    auto& eng = Engine::GetInstance();
    eng.Init();

    while (eng.IsRunning() && !WindowShouldClose())
    {
        eng.HandleEvents();
        eng.Update();
        eng.Render();
    }
 
    eng.Clean();
    return 0;
}