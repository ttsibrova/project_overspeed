#pragma once
#include <string>
#include <vector>

namespace map {

class Layer
{
public:
    Layer (std::vector<uint32_t>&& tileIDs, std::string name):
        m_tileIDs (std::move (tileIDs)),
        m_name (std::move (name))
    {}

    inline const std::vector<uint32_t>& getTiles() const { return m_tileIDs; }

    inline size_t      getTilesNum() const { return m_tileIDs.size(); }
    inline std::string getName() const { return m_name; }

private:
    const std::vector<uint32_t> m_tileIDs;
    std::string                 m_name;
};

} // namespace map
