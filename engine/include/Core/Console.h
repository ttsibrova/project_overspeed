#pragma once
#include <string>

namespace core {

class Console
{
public:

    void draw();

private:
    std::string m_buffer;
};

}
