#include <Map/RegisteredMaps.h>
#include <Tools/CompileTimeFunctions.h>
#include <cassert>
#include <vector>
#include <utility>

namespace map
{

std::string GetPath (RegisteredMap map)
{
    std::string path ("assets/lvl_");
    switch (map)
    {
    case RegisteredMap::level_1:
        path+="1_tmp";
        break;
    default:
        assert (false);
        break;
    }
    return path + ".tmx";
}

}

