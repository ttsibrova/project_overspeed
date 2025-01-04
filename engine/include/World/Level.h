#pragma once
#include <Map/Layer.h>
#include <Map/RegisteredMaps.h>
#include <Map/Tileset.h>
#include <optional>
#include <limits>
#include <mdspan>
#include <string>
#include <vector>

namespace tinytmx {
class Map;
}

struct GroundData
{
    std::mdspan <const int,
        std::extents <size_t,
        std::numeric_limits <size_t>::max(),
        std::numeric_limits <size_t>::max()>
    > tiles;
    float tile_height;
    float tile_width;
};

class Level
{
public:
    static std::optional <Level> CreateLevel (Maps map);

    //Level (const Level&) = delete;
    //Level (Level&&) = default;
    void Draw();

    GroundData GetGroundData();

private:
    Level () = default;
    Level (const tinytmx::Map& tmxMap);

private:
    std::vector <Layer> m_layers;
    CollectionTileset   m_cTileset;
    EmbeddedTileset     m_eTileset;
    unsigned int        m_levelHeightInTiles {1};
};
