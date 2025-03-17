#pragma once
#include <Geom/Point2D.h>

#include <cstdint>
#include <string>
#include <vector>

namespace map {

enum class ActuatorType : uint16_t
{
    INCOMPATIBLE = 0,

    PEDESTAL_ACTIVE,
    PEDESTAL_INACTIVE,
};

// Implied that Actuators graphic-wise are
// belong to collection tileset, so size is known and
// defined by Actuator's sprite
struct Actuator
{
    ActuatorType              type    = ActuatorType::INCOMPATIBLE;
    unsigned int              tileGid = 0;
    geom::Point2D             pos;
    std::vector<unsigned int> childIDs;
};

bool isActuator (const std::string& type);

} // namespace map
