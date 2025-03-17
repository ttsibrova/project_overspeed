#pragma once
#include <map/Actuator.h>

#include <ranges>
#include <unordered_map>

namespace world
{
class LevelActuators
{
public:
    LevelActuators (const std::unordered_map<uint32_t, map::Actuator>& actuators):
        m_actuators (actuators)
    {}

    auto getActuators() const { return m_actuators | std::ranges::views::values; }

private:
    const std::unordered_map<uint32_t, map::Actuator>& m_actuators;
};

}
