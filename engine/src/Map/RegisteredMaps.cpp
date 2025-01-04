#include <Map/RegisteredMaps.h>
#include <Tools/CompileTimeFunctions.h>
#include <cassert>
#include <vector>
#include <utility>

namespace maps
{

std::string GetPath (Maps map)
{
    std::string path ("assets/lvl_");
    switch (map)
    {
    case Maps::level_1:
        path+="1_swamp";
        break;
    default:
        assert (false);
        break;
    }
    return path + ".tmx";
}

}

