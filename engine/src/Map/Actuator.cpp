#include <Map/Actuator.h>

#include <unordered_map>

namespace map {
namespace {
enum class TmxActuator
{
    NONE,
    PEDESTAL,
};

const std::unordered_map<std::string, TmxActuator>& getTypeMap()
{
    static std::unordered_map<std::string, TmxActuator> map = {
        { "pedestal", TmxActuator::PEDESTAL }
    };
    return map;
}
}


ActuatorType getToggledActuatorType (ActuatorType type)
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

bool isActuator (const std::string& type)
{
    return getTypeMap().contains(type);
}
} // namespace map
