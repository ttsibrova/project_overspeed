#pragma once
#include <string>

namespace map
{

enum class RegisteredMap: uint8_t
{
    level_1 = 1,
};

std::string GetPath (RegisteredMap map);

}
