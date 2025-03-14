#include <map/TmxObjectConversion.h>

#include <Tools/Assert.h>

#include <tinytmx/tinytmxObject.hpp>
#include <tinytmx/tinytmxPropertySet.hpp>

#include <optional>

namespace map {
namespace {
enum class TmxActuator : uint8_t
{
    NONE,
    PEDESTAL,
};

std::unordered_map<std::string, TmxActuator> getActuatorTypeMap()
{
    std::unordered_map<std::string, TmxActuator> map = {
        { "pedestal", TmxActuator::PEDESTAL },
    };
    return map;
}

enum class TmxInteractableTile : uint8_t
{
    NONE,
    SLIDER_BEGIN,
    SLIDER_END,
    JUMP_BEGIN,
    JUMP_END
};

std::unordered_map<std::string, TmxInteractableTile> getInteractableTileTypeMap()
{
    std::unordered_map<std::string, TmxInteractableTile> map = {
        { "sliderBegin", TmxInteractableTile::SLIDER_BEGIN },
        { "sliderEnd", TmxInteractableTile::SLIDER_END },
        { "jumpBegin", TmxInteractableTile::JUMP_BEGIN },
        { "jumpEnd", TmxInteractableTile::JUMP_END },
    };
    return map;
}

template <class T>
std::unordered_map<int, std::pair<T, const tinytmx::Object*>>
    getFirstPassMap (const std::vector<tinytmx::Object*>& tmxObjects, const std::unordered_map<std::string, T>& typeMap)
{
    std::unordered_map<int, std::pair<T, const tinytmx::Object*>> idToTypeMap;
    for (const auto* object : tmxObjects) {
        auto findResult = typeMap.find (object->GetType());
        if (findResult != typeMap.end()) {
            idToTypeMap.emplace (object->GetID(), std::make_pair (findResult->second, object));
        }
    }

    return idToTypeMap;
}

std::optional<InteractableTile> fillInteractableTile (
    const std::unordered_map<int, std::pair<TmxInteractableTile, const tinytmx::Object*>>& idToInteractableTileMap,
    const tinytmx::Object* ptr, const GridTileSize& tileSize, InteractableTileType activeTile)
{
    auto properties       = ptr->GetProperties();
    auto interactableType = properties->GetBoolProperty ("active") ? activeTile : getFlippedActiveState (activeTile);

    TiledGridPositon begin {
        .x = static_cast<int> (ptr->GetX() / tileSize.width),
        .y = static_cast<int> (ptr->GetY() / tileSize.height),
    };

    auto endIt = idToInteractableTileMap.find (properties->GetObjectProperty ("end"));
    m_assert (endIt != idToInteractableTileMap.end(), "Missing end object");
    TiledGridPositon end {};
    if (endIt != idToInteractableTileMap.end()) {
        auto& sliderEnd = endIt->second.second;

        end.x = static_cast<int> (sliderEnd->GetX() / tileSize.width);
        end.y = static_cast<int> (sliderEnd->GetY() / tileSize.height);
    }
    else {
        return std::nullopt;
    }

    InteractableTile filledTile {
        .type  = interactableType,
        .begin = begin,
        .end   = end,
    };

    return filledTile;
}

void connectTileToActuator (const tinytmx::PropertySet* tileProperties, int tileId, std::unordered_map<int, Actuator>& actuators)
{
    int  actuatorId = tileProperties->GetObjectProperty ("actuator");
    auto actuatorIt = actuators.find (actuatorId);
    if (actuatorIt != actuators.end()) {
        actuatorIt->second.childIDs.push_back (tileId);
    }
}

} // namespace

TmxObjectConversionResult convertTmxObjects (const std::vector<tinytmx::Object*>& tmxObjects, GridTileSize tileSize)
{
    const auto idToActuatorMap         = getFirstPassMap (tmxObjects, getActuatorTypeMap());
    const auto idToInteractableTileMap = getFirstPassMap (tmxObjects, getInteractableTileTypeMap());

    std::unordered_map<int, Actuator> actuators;
    for (const auto& [key, value] : idToActuatorMap) {
        const auto& [type, ptr] = value;
        switch (type) {
        case TmxActuator::PEDESTAL:
        {
            auto properties   = ptr->GetProperties();
            auto actuatorType = properties->GetBoolProperty ("active") ? ActuatorType::PEDESTAL_ACTIVE
                                                                       : ActuatorType::PEDESTAL_INACTIVE;

            actuators[key] = Actuator { .type = actuatorType };
            break;
        }
        default:
            m_assert (false, "Not accounted enum value detected");
        }
    }

    std::unordered_map<int, InteractableTile> interactableTiles;
    for (const auto& [key, value] : idToInteractableTileMap) {
        const auto& [type, ptr] = value;
        switch (type) {
        case TmxInteractableTile::SLIDER_END:
        case TmxInteractableTile::JUMP_END:
            continue;
        case TmxInteractableTile::SLIDER_BEGIN:
        {
            auto filledTile =
                fillInteractableTile (idToInteractableTileMap, ptr, tileSize, InteractableTileType::SLIDER_ACTIVE);
            if (filledTile.has_value()) {
                interactableTiles[key] = std::move (filledTile.value());
            }
            else {
                continue;
            }
            connectTileToActuator (ptr->GetProperties(), key, actuators);
            break;
        }
        case TmxInteractableTile::JUMP_BEGIN:
        {
            auto filledTile = fillInteractableTile (idToInteractableTileMap, ptr, tileSize, InteractableTileType::JUMP_ACTIVE);
            if (filledTile.has_value()) {
                interactableTiles[key] = std::move (filledTile.value());
            }
            else {
                continue;
            }
            connectTileToActuator (ptr->GetProperties(), key, actuators);
            break;
        }
        default:
            continue;
        }
    }

    TmxObjectConversionResult result {
        .idToActuatorsMap        = std::move (actuators),
        .idToInteractableTileMap = std::move (interactableTiles),
    };
    return result;
}

} // namespace map
