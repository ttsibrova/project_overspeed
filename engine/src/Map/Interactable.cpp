#include <Map/Interactable.h>

#include <tools/Assert.h>

#include <tinytmxObject.hpp>
#include <tinytmxPropertySet.hpp>

#include <ranges>

namespace map {

InteractableTileType getToggledInteractableTileType (const InteractableTileType source)
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

} // namespace map
