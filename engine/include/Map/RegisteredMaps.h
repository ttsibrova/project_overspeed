#pragma once
#include <string>

enum class Maps: uint8_t
{
    level_1 = 1,
};

namespace maps
{
std::string GetPath (Maps map);

}
