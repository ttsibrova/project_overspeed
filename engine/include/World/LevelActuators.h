#pragma once
#include <Core/Logger.h>
#include <Graphics/TextureManager.h>
#include <Map/Actuator.h>
#include <Map/Tileset.h>
#include <World/Settings.h>

#include <ranges>
#include <unordered_map>

namespace world
{
class LevelActuators
{
public:
    LevelActuators (const std::unordered_map<uint32_t, map::Actuator>& actuators,
                    const map::CollectionTileset& cTileset):
        m_actuators (actuators),
        m_cTileset (cTileset)
    {}

    auto getActuatorIds() const { return m_actuators | std::ranges::views::keys; }
    auto getActuatorValues() const { return m_actuators | std::ranges::views::values; }
    auto getActuatorsColliders() const {
        auto actuatorCollider = [this] (const map::Actuator& actuator) {
            // fallback sizes in case if image is missing
            float height = settings::tileSize.height;
            float width  = settings::tileSize.width;
            if (m_cTileset.IsTileBelongsToSet(actuator.tileGid)) {
                auto imageInfo = TextureManager::getInstance().getImageInfo(m_cTileset.GetImageID(actuator.tileGid));
                height         = imageInfo.height;
                width          = imageInfo.width;
            }
            else {
                core::log::warning ("Texture is missing for actuator"); // SHOULD I have IDs inside of actuator?
            }
            return physics::Collider (actuator.pos, height, width);
        };
        return getActuatorValues() | std::ranges::views::transform (actuatorCollider);
    }

private:
    const std::unordered_map<uint32_t, map::Actuator>& m_actuators;
    const map::CollectionTileset&                      m_cTileset;
};

}
