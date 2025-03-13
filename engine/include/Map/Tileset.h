#pragma once
#include <vector>

namespace map {

class Tileset
{
public:
    Tileset():
        m_startID (0),
        m_numElems (0)
    {}

    Tileset (int startID, int numElems):
        m_startID (startID),
        m_numElems (numElems)
    {}

    inline int GetStartID() const { return m_startID; }

    inline bool IsTileBelongsToSet (const int tileID) const
    {
        return tileID >= m_startID && tileID < (m_startID + m_numElems);
    }

private:
    int m_startID;
    int m_numElems;
};

class CollectionTileset: public Tileset
{
public:
    CollectionTileset() {}

    CollectionTileset (std::vector<size_t>&& imagePathIDs, int startID, int numElems):
        Tileset (startID, numElems),
        m_imagePathIDs (std::move (imagePathIDs))
    {}

    inline size_t GetImageID (int tileID) const { return m_imagePathIDs[tileID - GetStartID()]; }

private:
    std::vector<size_t> m_imagePathIDs;
};

struct TilePos
{
    int col { 0 };
    int row { 0 };
};

class EmbeddedTileset: public Tileset
{
public:
    EmbeddedTileset():
        m_numColumns (0),
        m_tileHeight (0),
        m_tileWidth (0),
        m_imagePathID (0)
    {}

    EmbeddedTileset (int numColumns, float tileHeight, float tileWidth, size_t imagePathID, int startID, int numElems):
        Tileset (startID, numElems),
        m_numColumns (numColumns),
        m_tileHeight (tileHeight),
        m_tileWidth (tileWidth),
        m_imagePathID (imagePathID)
    {}

    inline TilePos GetTilePosition (const int TileID) const
    {
        return { (TileID % m_numColumns), (TileID / m_numColumns) - 1 };
    }

    inline std::pair<float, float> GetHeightWidth() const { return { m_tileHeight, m_tileWidth }; }

    inline size_t GetImageID() const { return m_imagePathID; }

private:
    int    m_numColumns;
    float  m_tileHeight;
    float  m_tileWidth;
    size_t m_imagePathID;
};

} // namespace map
