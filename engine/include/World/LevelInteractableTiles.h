#pragma once
#include <Geom/Point2D.h>
#include <Map/Interactable.h>
#include <Map/TiledGridPosition.h>
#include <Physics/Collider.h>

#include <ranges>
#include <unordered_map>

namespace world {

class LevelInteractableTiles
{
public:
    LevelInteractableTiles (const std::unordered_map<int, map::InteractableTile>& interactaleTiles,
                            map::GridTileSize tileSize):
        m_tiles (interactaleTiles),
        m_tileSize (std::move(tileSize))
    {}

    const map::GridTileSize& getGridTileSize() const { return m_tileSize; }

    auto getInteractableTiles() const
    { 
        return m_tiles | std::ranges::views::values;
    }

    auto /*range<std::pair<InteractableTile, physics::Collider>>*/ getInteractableTilesWithColliders() const
    {
        auto tileCollider = [tileSize = m_tileSize] (const map::InteractableTile& tile) {
            geom::Point2D min (tile.begin.x * tileSize.width - geom::precision::pixel,
                               (tile.begin.y - 1) * tileSize.height - geom::precision::pixel);
            geom::Point2D max ((tile.end.x + 1) * tileSize.width + 2 * geom::precision::pixel,
                               tile.end.y * tileSize.height + 2 * geom::precision::pixel);
            return std::pair (tile, physics::Collider (min, max));
        };

        return m_tiles | std::ranges::views::values | std::ranges::views::transform (tileCollider);
    }

private:
    const std::unordered_map<int, map::InteractableTile>& m_tiles;
    map::GridTileSize                                     m_tileSize;
};

}
