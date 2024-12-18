#pragma once
#include <Map/Layer.h>
#include <Map/TilesetImageData.h>
#include <vector>
#include <string>


class Level
{
public:
    Level () = default;
    Level (const Level&) = delete;

    bool Init (std::string mapFilePath);
    void Draw ();

private:
    std::vector <Layer> m_layers;
    CollectionTileset   m_cTileset;
    EmbeddedTileset     m_eTileset;
    unsigned int        m_levelWidhtInTiles {1};
};
