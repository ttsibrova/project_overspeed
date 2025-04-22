#include <Core/Engine.h>
#include <raylib.h>
#include <raygui.h>

int main()
{
    auto& eng = Engine::getInstance();
    eng.Init();

    while (eng.IsRunning() && !WindowShouldClose()) {
        eng.HandleEvents();
        eng.update();
        eng.Render();

        if (IsKeyPressed(KEY_F)) {
            ToggleFullscreen();
        }
    }

    eng.clean();
    return 0;
}
