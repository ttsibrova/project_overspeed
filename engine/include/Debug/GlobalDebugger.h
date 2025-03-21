#pragma once

namespace Debug {

class GlobalDebugger
{
public:
    static GlobalDebugger& getInstance();

    GlobalDebugger (const GlobalDebugger&)           = delete;
    GlobalDebugger operator= (const GlobalDebugger&) = delete;

public:
    void update();
    void draw();

private:
    GlobalDebugger() {}
};

} // namespace Debug
