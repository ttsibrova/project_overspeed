#pragma once
#include <Geom/Point2D.h>
#include <Map/Actuator.h>
#include <Map/Interactable.h>
#include <Map/Layer.h>
#include <Map/RegisteredMaps.h>
#include <Map/Tileset.h>
#include <World/LevelInteractableTiles.h>
#include <World/LevelActuators.h>
#include <limits>
#include <mdspan>
#include <optional>
#include <unordered_map>

namespace tinytmx {
class Map;
}

namespace world {

struct GroundData
{
    std::mdspan<const uint32_t, std::extents<size_t, std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max()>> tiles;
};

class Level
{
public:
    static std::optional<Level> createLevel (map::RegisteredMap map);

    Level (Level&&)                 = default;
    Level& operator= (Level&&)      = default;
    Level (const Level&)            = delete;
    Level& operator= (const Level&) = delete;

    void draw();

    GroundData getGroundData() const;
    LevelInteractableTiles getLevelInteractableTiles() const;
    LevelActuators getLevelActuators() const;

    void toggleActuator (uint32_t actuatorId);
    //void toggleInteractableTile (uint32_t tileId);

private:
    Level() = default;
    Level (const tinytmx::Map& tmxMap);

    void drawTileLayer (const map::Layer& layer) const;
    map::types::OptRefEmbeddedTileset findTileset (std::string name) const;

private:
    std::vector<map::Layer>                             m_layers;
    std::unordered_map<uint32_t, map::InteractableTile> m_interactableTiles;
    std::unordered_map<uint32_t, map::Actuator>         m_actuators;
    map::CollectionTileset                              m_cTileset;
    std::vector<map::EmbeddedTileset>                   m_eTilesets; // TODO: REPLACE LATER WITH ARRAY
    uint32_t                                            m_levelHeightInTiles { 1 };
};
} // namespace world
