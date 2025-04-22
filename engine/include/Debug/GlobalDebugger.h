#pragma once

namespace debug {

class GlobalDebugger
{
public:
    static GlobalDebugger& getInstance();

    GlobalDebugger (const GlobalDebugger&)           = delete;
    GlobalDebugger operator= (const GlobalDebugger&) = delete;

public:
    void update();
    void draw() const;

private:
    GlobalDebugger() {}
};

} // namespace debug
