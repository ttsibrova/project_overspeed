#pragma once
#include <vector>

namespace map {

class Layer
{
public:
    Layer (std::vector <int>&& tileIDs):
        m_tileIDs (std::move (tileIDs))
    {}

    inline const std::vector <int>& getTiles() const { return m_tileIDs; }
    inline size_t getTilesNum() const { return m_tileIDs.size(); }

private:
    const std::vector <int> m_tileIDs;
};

}
