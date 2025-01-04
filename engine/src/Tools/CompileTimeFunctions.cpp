#include <Tools/CompileTimeFunctions.h>

consteval size_t tools::Hash (std::string str)
{
    constexpr size_t prime{0x100000001B3};
    std::size_t result{0xcbf29ce484222325};

    for (std::size_t i = 0, ie = str.size(); i != ie; ++i)
        result = (result * prime) ^ str[i];

    return result;
}

