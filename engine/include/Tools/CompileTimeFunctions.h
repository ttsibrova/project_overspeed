#pragma once
#include <string>

namespace tools {

consteval size_t operator"" _hash (const char* str, size_t length);

struct HasherFNV1a
{
    size_t operator() (const std::string& str) const;
};

}


