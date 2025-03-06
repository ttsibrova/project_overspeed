#pragma once
#include <cstdint>
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
    ActuatorType     type;
    std::vector<int> childIDs;
};

}
