#include <World/Level.h>

#include <Graphics/TextureManager.h>
#include <tinytmx/tinytmxDataChunkTile.hpp>
#include <tinytmx/tinytmxImage.hpp>
#include <tinytmx/tinytmxMap.hpp>
#include <tinytmx/tinytmxTile.hpp>
#include <tinytmx/tinytmxTileLayer.hpp>
#include <tinytmx/tinytmxTileset.hpp>
#include <cassert>

#include <print>

CollectionTileset LoadCollectionTileset (const tinytmx::Tileset& tileset)
{
    std::vector <size_t> imgHashes;
    for (auto tile : tileset.GetTiles()) {
        auto img = tile->GetImage();
        assert (img->GetSource() != "");
        if (img->GetSource() != "") {
            imgHashes.push_back (TextureManager::GetInstance().Load (img->GetSource()));
        }
    }

    CollectionTileset res (std::move (imgHashes),
                           tileset.GetFirstGid(),
                           tileset.GetTileCount());
    return res;
}

EmbeddedTileset LoadEmbeddedTileset (const tinytmx::Tileset& tileset)
{
    auto image = tileset.GetImage();
    assert (image->GetSource() != "");

    size_t imgHash = TextureManager::GetInstance().Load (image->GetSource());

    EmbeddedTileset res (tileset.GetColumns(),
                         static_cast <float> (tileset.GetTileHeight()),
                         static_cast <float> (tileset.GetTileWidth()),
                         imgHash,
                         tileset.GetFirstGid(),
                         tileset.GetTileCount());
    return res;
}

Level::Level (const tinytmx::Map& tmxMap)
{
    assert (tmxMap.GetTilesets().size() < 3);

    for (const auto& tileset : tmxMap.GetTilesets()) {
        auto image = tileset->GetImage();
        if (!image) {
            m_cTileset = LoadCollectionTileset (*tileset);
        }
        else {
            m_eTileset = LoadEmbeddedTileset (*tileset);
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
        std::vector <int> layerIDs;
        auto widthInTiles = finiteLayer->GetWidth();
        layerIDs.reserve (widthInTiles * m_levelHeightInTiles);
        for (unsigned int i = 0; i < widthInTiles; i++) {
            for (unsigned int j = 0; j < m_levelHeightInTiles; j++) {
                layerIDs.push_back (finiteLayer->GetTileGid (i, j));
            }
        }
        m_layers.emplace_back (std::move (layerIDs));
    }
}


std::optional <Level> Level::CreateLevel (Maps map)
{
    tinytmx::Map levelMap;
    levelMap.ParseFile (maps::GetPath (map));
    if (levelMap.HasError()) {
        std::println ("{}", levelMap.GetErrorText());
        return {};
    }
    return { Level (levelMap) };
}

void Level::Draw()
{
    auto [tileHeight, tileWidth] = m_eTileset.GetHeightWidth();
    for (const auto& layer : m_layers) {
        const auto& tileIDs = layer.Tiles();
        for (int i = 0; i < static_cast <int> (tileIDs.size()); i++) {
            const float x = static_cast <float> (i / m_levelHeightInTiles);
            const float y = static_cast <float> (i % m_levelHeightInTiles);
            if (m_eTileset.IsTileBelongsToSet (tileIDs[i])) {
                TextureManager::GetInstance().DrawTile (m_eTileset.GetImageID(),
                                                        {x * tileWidth, y * tileWidth}, //pixel position
                                                        tileWidth, tileHeight,
                                                        m_eTileset.GetTilePosition (tileIDs[i])); //position on texture
            } else if (m_cTileset.IsTileBelongsToSet (tileIDs[i])) {
                TextureManager::GetInstance().Draw (m_cTileset.GetImageID (tileIDs[i]), {x * tileWidth, y * tileWidth});
            }
        }

    }
}

GroundData Level::GetGroundData()
{
    const auto& groundLayer = m_layers[0];
    auto md = std::mdspan (groundLayer.Tiles().data(), groundLayer.TilesNum() / m_levelHeightInTiles, m_levelHeightInTiles);
    auto [heiht, width] = m_eTileset.GetHeightWidth();
    return {md, heiht, width};
}
