#include <Map/Actuator.h>

#include <unordered_map>

namespace map {

ActuatorType getToggledActuatorType (const ActuatorType type)
{
    switch (type) {
    case ActuatorType::Invalid:
        return ActuatorType::Invalid;
    case ActuatorType::PedestalActive:
        return ActuatorType::PedestalInactive;
    case ActuatorType::PedestalInactive:
        return ActuatorType::PedestalActive;
    }
    std::unreachable();
}

} // namespace map
