#include <Core/Engine.h>
#include <raylib/raylib.h>

int main()
{
    auto& eng = Engine::GetInstance();
    eng.Init();

    while (eng.IsRunning() && !WindowShouldClose()) {
        eng.HandleEvents();
        eng.update();
        eng.Render();

        if (IsKeyPressed(KEY_F)) {
            ToggleFullscreen();
        }
    }

    eng.Clean();
    return 0;
}
