#include <World/Level.h>

#include <Graphics/TextureManager.h>
#include <cassert>
#include <tinytmx/tinytmxDataChunkTile.hpp>
#include <tinytmx/tinytmxImage.hpp>
#include <tinytmx/tinytmxMap.hpp>
#include <tinytmx/tinytmxTile.hpp>
#include <tinytmx/tinytmxTileLayer.hpp>
#include <tinytmx/tinytmxTileset.hpp>

#include <print>
#include <unordered_set>
#include <ranges>

namespace world {
namespace {

map::CollectionTileset LoadCollectionTileset (const tinytmx::Tileset& tileset)
{
    std::vector<size_t> imgHashes;
    for (auto tile : tileset.GetTiles()) {
        auto img = tile->GetImage();
        assert (img->GetSource() != "");
        if (img->GetSource() != "") {
            imgHashes.push_back (TextureManager::GetInstance().Load (img->GetSource()));
        }
    }

    map::CollectionTileset res (std::move (imgHashes), tileset.GetFirstGid(), tileset.GetTileCount());
    return res;
}

map::EmbeddedTileset LoadEmbeddedTileset (const tinytmx::Tileset& tileset)
{
    auto image = tileset.GetImage();
    assert (image->GetSource() != "");

    size_t imgHash = TextureManager::GetInstance().Load (image->GetSource());

    map::EmbeddedTileset res (tileset.GetColumns(),
                              static_cast<float> (tileset.GetTileHeight()),
                              static_cast<float> (tileset.GetTileWidth()),
                              imgHash,
                              tileset.GetFirstGid(),
                              tileset.GetTileCount());
    return res;
}

enum class AdjacencyDirection : uint8_t
{
    UNDEFINED,
    RIGHT,
    DOWN
};

std::optional<int> findNextTileInDirection (const std::vector<int>& tiles,
                                               const unsigned int layerHeightInTiles,
                                               size_t lastIndex,
                                               AdjacencyDirection dir)
{
    int x = static_cast<int> (lastIndex / layerHeightInTiles);
    int y = static_cast<int> (lastIndex % layerHeightInTiles);

    const int x_max = static_cast <int> (tiles.size() / layerHeightInTiles) - 1;
    const int y_max = static_cast <int> (layerHeightInTiles) - 1;

    if (dir == AdjacencyDirection::RIGHT) {
        x += 1;
        if (x > x_max) {
            return {};
        }
        return x * layerHeightInTiles + y;
    }

    if (dir == AdjacencyDirection::DOWN) {
        y += 1;
        if (y > y_max) {
            return {};
        }
        return x * layerHeightInTiles + y;
    }
    return {};
}

std::vector<int> findAdjacentTiles (const std::vector<int>& tiles, const unsigned int layerHeightInTiles,
                                    size_t lastIndex, AdjacencyDirection dir, std::unordered_set<size_t>& cachedIndices)
{
    if (dir == AdjacencyDirection::UNDEFINED) {
        auto tryValueRight = findNextTileInDirection (tiles, layerHeightInTiles, lastIndex, AdjacencyDirection::RIGHT);
        if (tryValueRight.has_value() &&
            map::recognizeTileType (tiles[tryValueRight.value()]) == map::recognizeTileType (tiles[lastIndex])) {
            cachedIndices.insert(tryValueRight.value());
            auto otherTiles = findAdjacentTiles (tiles, layerHeightInTiles, tryValueRight.value(),
                                                 AdjacencyDirection::RIGHT, cachedIndices);
            otherTiles.push_back (tryValueRight.value());
            return otherTiles;
        }
        auto tryValueDown = findNextTileInDirection (tiles, layerHeightInTiles, lastIndex, AdjacencyDirection::DOWN);
        if (tryValueDown.has_value() &&
            map::recognizeTileType (tiles[tryValueDown.value()]) == map::recognizeTileType (tiles[lastIndex])) {
            cachedIndices.insert (tryValueDown.value());
            auto otherTiles = findAdjacentTiles (tiles, layerHeightInTiles, tryValueDown.value(),
                                                 AdjacencyDirection::DOWN, cachedIndices);
            otherTiles.push_back (tryValueDown.value());
            return otherTiles;
        }
    }
    else {
        auto nextTileIndex = findNextTileInDirection(tiles, layerHeightInTiles, lastIndex, dir);
        if (nextTileIndex.has_value()) {
            const auto nextTileIndex_v = nextTileIndex.value();
            const auto nextTileType    = map::recognizeTileType (tiles[nextTileIndex_v]);
            if (nextTileType == map::InteractableTileType::INCOMPATIBLE) {
                cachedIndices.insert (nextTileIndex_v);
                return {};
            }
            if (nextTileType != map::recognizeTileType (tiles[lastIndex])) {
                return {};
            }
            else {
                cachedIndices.insert(nextTileIndex_v);
                auto otherTiles = findAdjacentTiles (tiles, layerHeightInTiles, nextTileIndex_v, dir, cachedIndices);
                otherTiles.push_back (nextTileIndex_v);
                return otherTiles;
            }
        }
        else {
            return {};
        }
    }
    return {};
}

std::vector<map::InteractableTile> recognizeInteractableTiles (const map::Layer& groundLayer, const unsigned int layerHeightInTiles)
{
    std::unordered_set<size_t> cachedIndices;
    std::vector<map::InteractableTile> result;
    for (const auto& [i, id] : groundLayer.getTiles() | std::ranges::views::enumerate) {
        if (cachedIndices.contains (i)) {
            continue;
        }
        const auto tileType = map::recognizeTileType (id);
        if (tileType == map::InteractableTileType::INCOMPATIBLE) {
            cachedIndices.insert (i);
            continue;
        }
        else {
            const auto adjacentTilesIndices = findAdjacentTiles(groundLayer.getTiles(), layerHeightInTiles, i, AdjacencyDirection::UNDEFINED, cachedIndices);
            if (!adjacentTilesIndices.empty()) {
                int x_start = static_cast<int> (i / layerHeightInTiles);
                int y_start = static_cast<int> (i % layerHeightInTiles);

                int x_end = static_cast<int> (adjacentTilesIndices.front() / layerHeightInTiles);
                int y_end = static_cast<int> (adjacentTilesIndices.front() % layerHeightInTiles);

                map::InteractableTile tile { .type = tileType, .start = { x_start, y_start }, .end = { x_end, y_end } };
                result.push_back (std::move (tile));
            }
        }

    }
    return result;
}

} // namespace

Level::Level (const tinytmx::Map& tmxMap)
{
    //assert (tmxMap.GetTilesets().size() < 3);

    const auto& tilesets = tmxMap.GetTilesets();
    for (size_t i = 0; i < 2; i++) {
        auto image = tilesets[i]->GetImage();
        if (!image) {
            m_cTileset = LoadCollectionTileset (*tilesets[i]);
        }
        else {
            m_eTileset = LoadEmbeddedTileset (*tilesets[i]);
        }
    }

    auto layers = tmxMap.GetTileLayers();
    for (const auto& layer : layers) {
        auto finiteLayer = layer->GetDataTileFiniteMap();
        assert (finiteLayer);
        if (m_levelHeightInTiles == 1) {
            m_levelHeightInTiles = finiteLayer->GetHeight();
        }
        else { // delete later
            assert (m_levelHeightInTiles == finiteLayer->GetHeight());
        }
        std::vector<int> layerIDs;
        auto             widthInTiles = finiteLayer->GetWidth();
        layerIDs.reserve (widthInTiles * m_levelHeightInTiles);
        for (unsigned int i = 0; i < widthInTiles; i++) {
            for (unsigned int j = 0; j < m_levelHeightInTiles; j++) {
                layerIDs.push_back (finiteLayer->GetTileGid (i, j));
            }
        }
        m_layers.emplace_back (std::move (layerIDs));
    }

    m_interactableTiles = recognizeInteractableTiles (m_layers[1], m_levelHeightInTiles);
}

std::optional<Level> Level::createLevel (map::RegisteredMap map)
{
    tinytmx::Map levelMap;
    levelMap.ParseFile (map::GetPath (map));
    if (levelMap.HasError()) {
        std::println ("{}", levelMap.GetErrorText());
        return {};
    }
    return { Level (levelMap) };
}

void Level::draw()
{
    auto [tileHeight, tileWidth] = m_eTileset.GetHeightWidth();
    for (const auto& layer : m_layers) {
        const auto& tileIDs = layer.getTiles();
        for (int i = 0; i < static_cast<int> (tileIDs.size()); i++) {
            const float x = static_cast<float> (i / m_levelHeightInTiles);
            const float y = static_cast<float> (i % m_levelHeightInTiles);
            if (m_eTileset.IsTileBelongsToSet (tileIDs[i])) {
                TextureManager::GetInstance().DrawTile (m_eTileset.GetImageID(),
                                                        { x * tileWidth, y * tileWidth }, // pixel position
                                                        tileWidth, tileHeight,
                                                        m_eTileset.GetTilePosition (tileIDs[i])); // position on texture
            }
            else if (m_cTileset.IsTileBelongsToSet (tileIDs[i])) {
                auto imageId   = m_cTileset.GetImageID (tileIDs[i]);
                auto imageInfo = TextureManager::GetInstance().getImageInfo (imageId);
                TextureManager::GetInstance().draw (imageId, { x * tileWidth, (y + 1) * tileHeight - imageInfo.height });
            }
        }
    }
}

GroundData Level::getGroundData()
{
    const auto& groundLayer = m_layers[1];
    auto md = std::mdspan (groundLayer.getTiles().data(), groundLayer.getTilesNum() / m_levelHeightInTiles, m_levelHeightInTiles);
    auto [heiht, width] = m_eTileset.GetHeightWidth();
    return { md, heiht, width };
}

const std::vector<map::InteractableTile> Level::getInteractableTiles()
{
    return m_interactableTiles;
}

size_t GroundData::flattenTilePos (map::TilePos& tilePos)
{
    return tilePos.col * tiles.extent (1) + tilePos.row; // verify
}

} // namespace world
