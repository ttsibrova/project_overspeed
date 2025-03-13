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


bool isActuator (const std::string& type)
{
    return getTypeMap().contains(type);
}
} // namespace map
