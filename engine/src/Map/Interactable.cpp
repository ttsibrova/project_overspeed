#include <Map/Interactable.h>

#include <tools/Assert.h>

#include <tinytmxObject.hpp>
#include <tinytmxPropertySet.hpp>

#include <ranges>

namespace map {

namespace {
enum class TmxInteractable
{
    NONE,
    SLIDER_BEGIN,
    SLIDER_END,
    JUMP_BEGIN,
    JUMP_END
};

const std::unordered_map<std::string, TmxInteractable>& getTypeMap()
{
    static std::unordered_map<std::string, TmxInteractable> map = {
        { "sliderBegin", TmxInteractable::SLIDER_BEGIN },
        { "sliderEnd", TmxInteractable::SLIDER_END },
        { "jumpBegin", TmxInteractable::JUMP_BEGIN },
        { "jumpEnd", TmxInteractable::JUMP_END } 
    };
    return map;
}
}

InteractableTileType getFlippedActiveState (InteractableTileType source)
{
    switch (source) {
    case InteractableTileType::SLIDER_ACTIVE:
        return InteractableTileType::SLIDER_INACTIVE;
    case InteractableTileType::SLIDER_INACTIVE:
        return InteractableTileType::SLIDER_ACTIVE;
    case InteractableTileType::JUMP_ACTIVE:
        return InteractableTileType::JUMP_INACTIVE;
    case InteractableTileType::JUMP_INACTIVE:
        return InteractableTileType::JUMP_ACTIVE;
    default:
        assert ("false");
        // add new cases
        break;
    }
    std::unreachable();
}

InteractableTileType recognizeTileType (int tileId)
{
    switch (tileId) {
    case 4:
    case 5:
    case 6:
        return InteractableTileType::SLIDER_ACTIVE;
    case 14:
    case 15:
    case 16:
        return InteractableTileType::SLIDER_INACTIVE;
    case 47:
    case 48:
    case 49:
        return InteractableTileType::JUMP_ACTIVE;
    case 57:
    case 58:
    case 59:
        return InteractableTileType::JUMP_INACTIVE;
    default:
        return InteractableTileType::INCOMPATIBLE;
    };
}

bool isInteractableTile (const std::string& type)
{
    return getTypeMap().contains (type);
}

} // namespace map
