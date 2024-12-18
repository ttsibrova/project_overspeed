#pragma once
#include <vector>

class Layer
{
public:
    Layer (std::vector <int>&& tileIDs):
        m_tileIDs (std::move (tileIDs))
    {}

    inline const std::vector <int>& Tiles() const { return m_tileIDs; }
    inline size_t TilesNum() const { return m_tileIDs.size(); }

private:
    const std::vector <int> m_tileIDs;
};
