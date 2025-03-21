#pragma once
#include <Geom/Point2D.h>
#include <Map/Interactable.h>
#include <Map/TiledGridPosition.h>
#include <Physics/Collider.h>
#include <World/Settings.h>

#include <ranges>
#include <unordered_map>

namespace world {

class LevelInteractableTiles
{
public:
    LevelInteractableTiles (const std::unordered_map<uint32_t, map::InteractableTile>& interactaleTiles):
        m_tiles (interactaleTiles)
    {}

    auto getInteractableTiles() const
    { 
        return m_tiles | std::ranges::views::values;
    }

    auto /*range<std::pair<InteractableTile, physics::Collider>>*/ getInteractableTilesTypeWithColliders() const
    {
        auto tileCollider = [] (const map::InteractableTile& tile) {
            geom::Point2D min (tile.begin.x * settings::tileSize.width - geom::precision::pixel,
                               (tile.begin.y - 1) * settings::tileSize.height - geom::precision::pixel);
            geom::Point2D max ((tile.end.x + 1) * settings::tileSize.width + 2 * geom::precision::pixel,
                               tile.end.y * settings::tileSize.height + 2 * geom::precision::pixel);
            return std::pair (tile.type, physics::Collider (min, max));
        };

        return m_tiles | std::ranges::views::values | std::ranges::views::transform (tileCollider);
    }

private:
    const std::unordered_map<uint32_t, map::InteractableTile>& m_tiles;
};

}
