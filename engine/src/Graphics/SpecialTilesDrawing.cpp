#include <Graphics/SpecialTilesDrawing.h>
#include <Graphics/TextureManager.h>
#include <Map/Tileset.h>
#include <Tools/Assert.h>
#include <ranges>

namespace graphics {

namespace {
const map::EmbeddedTileset& getSpecialTilesTileset()
{
    auto init = []() {
        size_t imageId = TextureManager::GetInstance().Load ("assets/special_objects_animated-Sheet.png");
        map::EmbeddedTileset tileset (3, 32.f, 32.f, imageId, 0, 36, "special_objects");
        return tileset;
    };

    static map::EmbeddedTileset tileset = init();
    return tileset;
}

struct SpecialTileSpiteIds
{
    int begin  = 0;
    int middle = 0;
    int end    = 0;
};

SpecialTileSpiteIds getSpriteIds (map::InteractableTileType type)
{
    switch (type) {
    case map::InteractableTileType::SLIDER_ACTIVE:
        return SpecialTileSpiteIds { .begin = 0, .middle = 3, .end = 6 };
    case map::InteractableTileType::SLIDER_INACTIVE:
        return SpecialTileSpiteIds { .begin = 9, .middle = 12, .end = 15 };
    case map::InteractableTileType::JUMP_ACTIVE:
        return SpecialTileSpiteIds { .begin = 18, .middle = 21, .end = 24 };
    case map::InteractableTileType::JUMP_INACTIVE:
        return SpecialTileSpiteIds { .begin = 27, .middle = 30, .end = 33 };
    default:
        m_assert (false, "New cases not covered.");
    }
    return SpecialTileSpiteIds {};
}

}

void draw (const world::LevelInteractableTiles& tiles)
{
    auto& manager = TextureManager::GetInstance();
    const auto& tileSize = tiles.getGridTileSize();
    const auto& tileset = getSpecialTilesTileset();
    for (const auto& tile : tiles.getInteractableTiles()) {
        geom::Point2D pos(tile.begin.x * tileSize.width, (tile.begin.y - 1) * tileSize.height);
        SpecialTileSpiteIds ids = getSpriteIds (tile.type);
        manager.DrawTile (tileset.GetImageID(), pos, tileSize.width, tileSize.height, tileset.GetTilePosition (ids.begin));
        int nMidTiles = tile.end.x - tile.begin.x - 1;
        for (int i = 1; i < nMidTiles + 1; i++) {
            pos.X() += tileSize.width;
            manager.DrawTile (tileset.GetImageID(), pos, tileSize.width, tileSize.height, tileset.GetTilePosition (ids.middle));
        }
        pos.X() += tileSize.width;
        manager.DrawTile (tileset.GetImageID(), pos, tileSize.width, tileSize.height, tileset.GetTilePosition (ids.end));
    }
}

void draw (const world::LevelActuators& actuators, const map::CollectionTileset& tileset)
{
    auto& manager = TextureManager::GetInstance();
    for (const auto& actuator: actuators.getActuators()) {
        if (tileset.IsTileBelongsToSet(actuator.tileGid)) {
            manager.draw (tileset.GetImageID(actuator.tileGid), actuator.pos);
        }
    }
}

} // namespace graphics
