#include <World/Level.h>

#include <Graphics/TextureManager.h>
#include <Map/TiledGridPosition.h>
#include <Map/TmxObjectConversion.h>

#include <Tools/Assert.h>

#include <tinytmx/tinytmxDataChunkTile.hpp>
#include <tinytmx/tinytmxImage.hpp>
#include <tinytmx/tinytmxMap.hpp>
#include <tinytmx/tinytmxObject.hpp>
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
} // namespace

Level::Level (const tinytmx::Map& tmxMap)
{
    //assert (tmxMap.GetTilesets().size() < 3);

    const auto& tilesets = tmxMap.GetTilesets();
    for (size_t i = 0; i < tilesets.size(); i++) {
        auto image = tilesets[i]->GetImage();
        if (!image) {
            m_cTileset = LoadCollectionTileset (*tilesets[i]);
        }
        else {
            m_eTilesets.push_back (LoadEmbeddedTileset (*tilesets[i]));
        }
    }

    const auto& tileLayers = tmxMap.GetTileLayers();
    for (const auto* layer : tileLayers) {
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

    const auto& objectLayers = tmxMap.GetObjectGroups();
    m_assert (objectLayers.size() == 1, "More object layers than expected");

    for (const auto* layer : objectLayers) {
        auto [height, width] = m_eTilesets[0].GetHeightWidth(); // TODO: replace next iteration
        map::GridTileSize tileSize {
            .width  = width,
            .height = height,
        };
        auto conversionResult = map::convertTmxObjects (layer->GetObjects(), tileSize);
        m_actuators           = std::move (conversionResult.idToActuatorsMap);
        m_interactableTiles   = std::move (conversionResult.idToInteractableTileMap);
    }
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
    const auto& groundTileset    = getGroundTileset();
    auto [tileHeight, tileWidth] = groundTileset.GetHeightWidth();
    for (const auto& layer : m_layers) {
        const auto& tileIDs = layer.getTiles();
        for (int i = 0; i < static_cast<int> (tileIDs.size()); i++) {
            const float x = static_cast<float> (i / m_levelHeightInTiles);
            const float y = static_cast<float> (i % m_levelHeightInTiles);

            if (groundTileset.IsTileBelongsToSet (tileIDs[i])) {
                TextureManager::GetInstance().DrawTile (groundTileset.GetImageID(),
                                                        { x * tileWidth, y * tileWidth }, // pixel position
                                                        tileWidth, tileHeight,
                                                        groundTileset.GetTilePosition (tileIDs[i])); // position on texture
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
    auto [heiht, width] = getGroundTileset().GetHeightWidth();
    return { md, heiht, width };
}

LevelInteractableTiles Level::getLevelInteractableTiles()
{
    auto [height, width] = m_eTilesets[0].GetHeightWidth(); // TODO: replace
    map::GridTileSize tileSize {
        .width  = width,
        .height = height,
    };
    return LevelInteractableTiles (m_interactableTiles, std::move(tileSize));
}

size_t GroundData::flattenTilePos (map::TilePos& tilePos)
{
    return tilePos.col * tiles.extent (1) + tilePos.row; // verify
}

} // namespace world
