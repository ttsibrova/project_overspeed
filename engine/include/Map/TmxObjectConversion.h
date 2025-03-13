#pragma once
#include <map/Actuator.h>
#include <map/Interactable.h>

#include <unordered_map>

namespace tinytmx {
class Object;
}

namespace map {

struct TmxObjectConversionResult
{
    std::unordered_map<int, Actuator>         idToActuatorsMap;
    std::unordered_map<int, InteractableTile> idToInteractableTileMap;
};

TmxObjectConversionResult convertTmxObjects(const std::vector<tinytmx::Object*>& tmxObjects, GridTileSize tileSize);

}
