#include <World/Level.h>

#include <Graphics/TextureManager.h>
#include <Graphics/SpecialTilesDrawing.h>
#include <Map/TiledGridPosition.h>
#include <Map/TmxObjectConversion.h>
#include <World/Settings.h>

#include <Tools/Assert.h>

#include <tinytmxDataChunkTile.hpp>
#include <tinytmxImage.hpp>
#include <tinytmxMap.hpp>
#include <tinytmxObject.hpp>
#include <tinytmxTile.hpp>
#include <tinytmxTileLayer.hpp>
#include <tinytmxTileset.hpp>

#include <functional>
#include <print>

namespace world {
namespace {

map::CollectionTileset LoadCollectionTileset (const tinytmx::Tileset& tileset)
{
    std::unordered_map<int, size_t> imgHashes;
    for (auto tile : tileset.GetTiles()) {
        auto img = tile->GetImage();
        assert (img->GetSource() != "");
        if (img->GetSource() != "") {
            auto imgHash = TextureManager::getInstance().Load (img->GetSource());
            if (imgHash.has_value()) {
                imgHashes.emplace (tile->GetId(), imgHash.value());
            }
            else {
                imgHashes.emplace(tile->GetId(), TextureManager::getMissingId());
            }
        }
    }

    map::CollectionTileset res (std::move (imgHashes), tileset.GetFirstGid(), tileset.GetTileCount(), tileset.GetName());
    return res;
}

std::optional <map::EmbeddedTileset> LoadEmbeddedTileset (const tinytmx::Tileset& tileset)
{
    auto image = tileset.GetImage();
    assert (image->GetSource() != "");

    auto imgHash = TextureManager::getInstance().Load (image->GetSource());
    if (!imgHash.has_value()) {
        return std::nullopt;
    }

    m_assert (tileset.GetTileHeight() == settings::tileSize.height, "Tileset tile height is different from global settings");
    m_assert (tileset.GetTileWidth() == settings::tileSize.width, "Tileset tile width is different from global settings");

    map::EmbeddedTileset res (tileset.GetColumns(),
                              imgHash.value(),
                              tileset.GetFirstGid(),
                              tileset.GetTileCount(),
                              tileset.GetName());
    return res;
}

void drawLayerWithTileset (const map::Layer& layer, const map::CollectionTileset& layerTileset, uint32_t levelHeightInTiles)
{
    const float tileWidth  = 32.f;
    const float tileHeight = 32.f;

    const auto& tileIDs = layer.getTiles();
    for (int i = 0; i < static_cast<int> (tileIDs.size()); i++) {
        const float x = static_cast<float> (i / levelHeightInTiles);
        const float y = static_cast<float> (i % levelHeightInTiles);
        if (layerTileset.IsTileBelongsToSet (tileIDs[i])) {
            auto imageId   = layerTileset.GetImageID (tileIDs[i]);
            auto imageInfo = TextureManager::getInstance().getImageInfo (imageId);
            TextureManager::getInstance().draw (imageId, { x * tileWidth, (y + 1) * tileHeight - imageInfo.height });
        }
    }
}

void drawMissing (const map::Layer& layer, uint32_t levelHeightInTiles)
{
    float tileHeight = world::settings::tileSize.height;
    float tileWidth  = world::settings::tileSize.width;

    const auto& tileIDs = layer.getTiles();
    for (int i = 0; i < static_cast<int> (tileIDs.size()); i++) {
        const float x = static_cast<float> (i / levelHeightInTiles);
        const float y = static_cast<float> (i % levelHeightInTiles);

        if (tileIDs[i] != map::Tileset::getEmptyTileId()) {
            TextureManager::getInstance().drawMissing ({ x * tileWidth, y * tileWidth }, tileWidth, tileHeight, RED);
        }
    }
}
} // namespace

Level::Level (const tinytmx::Map& tmxMap)
{
    const auto& tilesets = tmxMap.GetTilesets();
    for (size_t i = 0; i < tilesets.size(); i++) {
        auto image = tilesets[i]->GetImage();
        if (!image) {
            m_cTileset = LoadCollectionTileset (*tilesets[i]);
        }
        else if (auto eTileset = LoadEmbeddedTileset (*tilesets[i])) {
            m_eTilesets.push_back (eTileset.value());
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
        std::vector<unsigned int> layerIDs;
        auto widthInTiles = finiteLayer->GetWidth();
        layerIDs.reserve (widthInTiles * m_levelHeightInTiles);
        for (unsigned int i = 0; i < widthInTiles; i++) {
            for (unsigned int j = 0; j < m_levelHeightInTiles; j++) {
                layerIDs.push_back (finiteLayer->GetTileGid (i, j));
            }
        }
        m_layers.emplace_back (std::move (layerIDs), layer->GetName());
    }

    const auto& objectLayers = tmxMap.GetObjectGroups();
    m_assert (objectLayers.size() == 1, "More object layers than expected");

    for (const auto* layer : objectLayers) {
        auto conversionResult = map::convertTmxObjects (layer->GetObjects());
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

void Level::drawTileLayer (const map::Layer& layer)
{
    std::string layerName = layer.getName();

    auto tileSetIt  = std::ranges::find (m_eTilesets, layerName, &map::EmbeddedTileset::getName);
    if (tileSetIt == m_eTilesets.end()) {
        return drawMissing(layer, m_levelHeightInTiles);
    }

    const float tileWidth  = world::settings::tileSize.width;
    const float tileHeight = world::settings::tileSize.height;

    const auto& tileIDs = layer.getTiles();
    for (int i = 0; i < static_cast<int> (tileIDs.size()); i++) {
        const float x = static_cast<float> (i / m_levelHeightInTiles);
        const float y = static_cast<float> (i % m_levelHeightInTiles);
        if (tileSetIt->IsTileBelongsToSet (tileIDs[i])) {
            TextureManager::getInstance().DrawTile (tileSetIt->GetImageID(), { x * tileWidth, y * tileWidth }, // pixel position
                                                    tileWidth, tileHeight,
                                                    tileSetIt->GetTilePosition (tileIDs[i])); // position on texture
        }
    }
}

map::types::OptRefEmbeddedTileset Level::findTileset (std::string name)
{
    auto tileSetIt = std::ranges::find (m_eTilesets, name, &map::EmbeddedTileset::getName);
    if (tileSetIt != m_eTilesets.end()) {
        return *tileSetIt;
    }
    return std::nullopt;
}


void Level::draw()
{
    using namespace std::placeholders;
    for (const auto& layer : m_layers) {
        if (layer.getName() == "ground") {
            drawTileLayer(layer);
        }
        else if (layer.getName() == "background") {
            drawTileLayer(layer);
        }
        else if (layer.getName() == "decorations") {
            drawLayerWithTileset (layer, m_cTileset, m_levelHeightInTiles);
        }
    }

    graphics::draw (getLevelInteractableTiles(), findTileset ("special_tiles"));
    graphics::draw (getLevelActuators(), m_cTileset);
}

GroundData Level::getGroundData()
{
    const auto& groundLayer = m_layers[1];
    auto md = std::mdspan (groundLayer.getTiles().data(), groundLayer.getTilesNum() / m_levelHeightInTiles, m_levelHeightInTiles);
    return { md };
}

LevelInteractableTiles Level::getLevelInteractableTiles()
{
    return LevelInteractableTiles (m_interactableTiles);
}

LevelActuators Level::getLevelActuators()
{
    return LevelActuators(m_actuators);
}

} // namespace world
