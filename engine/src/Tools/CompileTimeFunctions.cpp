#include <Tools/CompileTimeFunctions.h>
#include <string>

namespace tools {
namespace {

constexpr size_t hashString (const char* str, size_t length)
{
    size_t hash = 14695981039346656037ULL;
    for (size_t i = 0; i < length; ++i) {
        hash ^= static_cast<size_t> (str[i]);
        hash *= 1099511628211ULL;
    }
    return hash;
}

} // namespace

consteval size_t operator""_hash (const char* str, size_t length)
{
    return hashString (str, length);
}


size_t HasherFNV1a::operator() (const std::string& str) const
{
    return hashString (str.c_str(), str.length());
}

} // namespace tools
