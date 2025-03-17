#pragma once
#include <string>
#include <vector>

namespace map {

class Layer
{
public:
    Layer (std::vector<unsigned int>&& tileIDs, std::string name):
        m_tileIDs (std::move (tileIDs)),
        m_name (std::move (name))
    {}

    inline const std::vector<unsigned int>& getTiles() const { return m_tileIDs; }

    inline size_t      getTilesNum() const { return m_tileIDs.size(); }
    inline std::string getName() const { return m_name; }

private:
    const std::vector<unsigned int> m_tileIDs;
    std::string                     m_name;
};

} // namespace map
