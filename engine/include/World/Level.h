#pragma once
#include <geom/Point2D.h>
#include <Map/Actuator.h>
#include <Map/Interactable.h>
#include <Map/Layer.h>
#include <Map/RegisteredMaps.h>
#include <Map/Tileset.h>
#include <Map/TiledGridPosition.h>
#include <World/LevelInteractableTiles.h>
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
    std::mdspan<const int, std::extents<size_t, std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max()>> tiles;
    float tile_height;
    float tile_width;

    size_t flattenTilePos (map::TilePos& tilePos);
};

class Level
{
public:
    static std::optional<Level> createLevel (map::RegisteredMap map);

    // Level (const Level&) = delete;
    // Level (Level&&) = default;
    void draw();

    GroundData getGroundData();
    LevelInteractableTiles getLevelInteractableTiles();

private:
    Level() = default;
    Level (const tinytmx::Map& tmxMap);

    inline const map::EmbeddedTileset& getGroundTileset() const { return m_eTilesets[0]; }

private:
    std::vector<map::Layer>                        m_layers;
    std::unordered_map<int, map::InteractableTile> m_interactableTiles;
    std::unordered_map<int, map::Actuator>         m_actuators;
    map::CollectionTileset                         m_cTileset;
    std::vector<map::EmbeddedTileset>              m_eTilesets; // TODO: REPLACE LATER WITH ARRAY
    unsigned int                                   m_levelHeightInTiles { 1 };
};
} // namespace world
