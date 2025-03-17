#pragma once
#include <string>

namespace map {

class Tileset
{
public:
    Tileset():
        m_startGid (0),
        m_numElems (0)
    {}

    Tileset (int startGid, int numElems, std::string name):
        m_startGid (startGid),
        m_numElems (numElems),
        m_name (std::move (name))
    {}

    inline int getStartGid() const { return m_startGid; }
    inline int getNumElements() const { return m_numElems; }
    inline int getTileId (int tileGid) const { return tileGid - m_startGid; }
    inline std::string getName() const { return m_name; }

private:
    int         m_startGid;
    int         m_numElems;
    std::string m_name;
};

class CollectionTileset: public Tileset
{
public:
    CollectionTileset() {}

    CollectionTileset (std::unordered_map<int, size_t>&& imagePathIDs, int startGid, int numElems, const std::string& name):
        Tileset (startGid, numElems, name),
        m_imagePathIDs (std::move (imagePathIDs))
    {}

    inline bool IsTileBelongsToSet (const int tileGid) const
    {
        return m_imagePathIDs.contains (tileGid - getStartGid());
    }

    inline size_t GetImageID (int tileGid) const { return m_imagePathIDs.at (getTileId(tileGid)); }

private:
    std::unordered_map<int, size_t> m_imagePathIDs;
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

    EmbeddedTileset (int numColumns, float tileHeight, float tileWidth,
                     size_t imagePathID,
                     int startGid, int numElems,
                     const std::string& name):
        Tileset (startGid, numElems, name),
        m_numColumns (numColumns),
        m_tileHeight (tileHeight),
        m_tileWidth (tileWidth),
        m_imagePathID (imagePathID)
    {}

    inline TilePos GetTilePosition (const int tileGid) const
    {
        return { (getTileId (tileGid)) % m_numColumns, (getTileId (tileGid)) / m_numColumns };
    }

    inline std::pair<float, float> GetHeightWidth() const { return { m_tileHeight, m_tileWidth }; }

    inline bool IsTileBelongsToSet (const int tileGid) const
    {
        return tileGid >= getStartGid() && tileGid < (getStartGid() + getNumElements());
    }

    inline size_t GetImageID() const { return m_imagePathID; }

private:
    int    m_numColumns;
    float  m_tileHeight;
    float  m_tileWidth;
    size_t m_imagePathID;
};

} // namespace map
