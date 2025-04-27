#include <Map/Interactable.h>

#include <Tools/Assert.h>

#include <tinytmxObject.hpp>
#include <tinytmxPropertySet.hpp>

#include <ranges>

namespace map {

InteractableTileType getToggledInteractableTileType (const InteractableTileType source)
{
    switch (source) {
    case InteractableTileType::SliderActive:
        return InteractableTileType::SliderInactive;
    case InteractableTileType::SliderInactive:
        return InteractableTileType::SliderActive;
    case InteractableTileType::JumpActive:
        return InteractableTileType::JumpInactive;
    case InteractableTileType::JumpInactive:
        return InteractableTileType::JumpActive;
    default:
        assert ("false");
        // add new cases
        break;
    }
    std::unreachable();
}

} // namespace map
