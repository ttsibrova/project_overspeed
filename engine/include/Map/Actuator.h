#pragma once
#include <Geom/Point2D.h>

#include <cstdint>
#include <string>
#include <vector>

namespace map {

enum class ActuatorType : uint16_t
{
    Invalid = 0,

    PedestalActive,
    PedestalInactive,
};

// Implied that Actuators graphic-wise are
// belong to collection tileset, so size is known and
// defined by Actuator's sprite
struct Actuator
{
    ActuatorType              type    = ActuatorType::Invalid;
    unsigned int              tileGid = 0;
    geom::Point2D             pos;
    std::vector<unsigned int> childIDs;
};

ActuatorType getToggledActuatorType (const ActuatorType type);

} // namespace map
