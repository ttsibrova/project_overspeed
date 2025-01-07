#pragma once

namespace Debug
{
class GlobalDebugger
{
public:
    static GlobalDebugger& GetInstance();

    GlobalDebugger (const GlobalDebugger&) = delete;
    GlobalDebugger operator= (const GlobalDebugger&) = delete;

public:
    void Update();
    void Draw();

private:
    GlobalDebugger(){}
};
}
