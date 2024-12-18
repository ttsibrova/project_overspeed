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


bool Level::Init (std::string mapFilePath)
{
    tinytmx::Map levelMap;
    levelMap.ParseFile (mapFilePath);
    if (levelMap.HasError()) {
        std::println ("{}", levelMap.GetErrorText());
        return false;
    }

    assert (levelMap.GetTilesets().size() < 3);

    for (const auto& tileset: levelMap.GetTilesets()) {
        auto image = tileset->GetImage();
        if (!image) {
            m_cTileset = LoadCollectionTileset (*tileset);
        } else {
            m_eTileset = LoadEmbeddedTileset (*tileset);
        }
    }

    auto layers = levelMap.GetTileLayers();
    for (const auto& layer : layers) {
        auto finiteLayer = layer->GetDataTileFiniteMap();
        assert (finiteLayer);
        if (m_levelWidhtInTiles == 1) {
            m_levelWidhtInTiles = finiteLayer->GetWidth();
        } else { // delete later
            assert (m_levelWidhtInTiles == finiteLayer->GetWidth());
        }
        std::vector <int> layerIDs;
        auto heightInTiles = finiteLayer->GetHeight();
        layerIDs.reserve (heightInTiles * m_levelWidhtInTiles);
        for (unsigned int i = 0; i < heightInTiles; i++) {
            for (unsigned int j = 0; j < m_levelWidhtInTiles; j++) {
                layerIDs.push_back (finiteLayer->GetTileGid (j, i));
            }
        }
        m_layers.emplace_back (std::move (layerIDs));
    }
    return true;
}

void Level::Draw()
{
    auto [tileHeight, tileWidth] = m_eTileset.GetHeightWidth();
    for (const auto& layer : m_layers) {
        const auto& tileIDs = layer.Tiles();
        for (int i = 0; i < static_cast <int> (tileIDs.size()); i++) {
            const float x = static_cast <float> (i % m_levelWidhtInTiles);
            const float y = static_cast <float> (i / m_levelWidhtInTiles);
            if (m_eTileset.IsTileBelongsToSet (tileIDs[i])) {
                TextureManager::GetInstance().DrawTile (m_eTileset.GetImageID(), {x * tileWidth, y * tileWidth}, tileWidth, tileHeight, m_eTileset.GetTilePosition (tileIDs[i]));
            } else if (m_cTileset.IsTileBelongsToSet (tileIDs[i])) {
                TextureManager::GetInstance().Draw (m_cTileset.GetImageID (tileIDs[i]), {x * tileWidth, y * tileWidth});
            }
        }

    }
}
