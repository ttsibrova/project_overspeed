#include <Map/Actuator.h>

#include <unordered_map>

namespace map {

ActuatorType getToggledActuatorType (const ActuatorType type)
{
    switch (type) {
    case ActuatorType::INCOMPATIBLE:
        return ActuatorType::INCOMPATIBLE;
    case ActuatorType::PEDESTAL_ACTIVE:
        return ActuatorType::PEDESTAL_INACTIVE;
    case ActuatorType::PEDESTAL_INACTIVE:
        return ActuatorType::PEDESTAL_ACTIVE;
    }
    std::unreachable();
}

} // namespace map
