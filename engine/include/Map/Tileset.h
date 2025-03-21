#pragma once
#include <Map/TiledGridPosition.h>
#include <string>

#include <optional>
#include <functional>

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

    static inline uint32_t getEmptyTileId() { return 0u; }

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

class EmbeddedTileset: public Tileset
{
public:
    EmbeddedTileset():
        m_numColumns (0),
        m_imagePathID (0)
    {}

    EmbeddedTileset (int numColumns, size_t imagePathID,
                     int startGid, int numElems,
                     const std::string& name):
        Tileset (startGid, numElems, name),
        m_numColumns (numColumns),
        m_imagePathID (imagePathID)
    {}

    inline map::TiledGridPositon GetTilePosition (const int tileGid) const
    {
        return { .x = (getTileId (tileGid)) % m_numColumns, .y = (getTileId (tileGid)) / m_numColumns };
    }

    inline map::TiledGridPositon GetTilePositionById (const int tileId) const
    {
        return { .x = tileId % m_numColumns, .y = tileId / m_numColumns };
    }

    inline bool IsTileBelongsToSet (const int tileGid) const
    {
        return tileGid >= getStartGid() && tileGid < (getStartGid() + getNumElements());
    }

    inline size_t GetImageID() const { return m_imagePathID; }

private:
    int    m_numColumns;
    size_t m_imagePathID;
};

namespace types
{

using OptRefEmbeddedTileset = std::optional<std::reference_wrapper<const EmbeddedTileset>>;
using OptRefCollectionTileset = std::optional<std::reference_wrapper<const CollectionTileset>>;

}

} // namespace map
