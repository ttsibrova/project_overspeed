#pragma once
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
    ActuatorType     type = ActuatorType::INCOMPATIBLE;
    std::vector<int> childIDs;
};

bool isActuator (const std::string& type);

} // namespace map
