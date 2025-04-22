#include <Map/RegisteredMaps.h>
#include <Tools/CompileTimeFunctions.h>
#include <cassert>
#include <utility>
#include <vector>

namespace map {

std::string GetPath (const RegisteredMap map)
{
    std::string path ("assets/lvl_");
    switch (map) {
    case RegisteredMap::level_1:
        path += "1_tmp";
        break;
    default:
        assert (false);
        break;
    }
    return path + ".tmx";
}

} // namespace map
