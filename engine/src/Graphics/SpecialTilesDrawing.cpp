#include <Graphics/SpecialTilesDrawing.h>
#include <Graphics/TextureManager.h>
#include <Map/Tileset.h>
#include <Tools/Assert.h>
#include <ranges>

namespace graphics {

namespace {

using Tile = map::InteractableTileType;

struct SpecialTileSpiteIds
{
    int begin  = 0;
    int middle = 0;
    int end    = 0;
};

SpecialTileSpiteIds getSpriteIds (map::InteractableTileType type)
{
    switch (type) {
    case Tile::SliderActive  : return SpecialTileSpiteIds { .begin = 0,  .middle = 3,  .end = 6  };
    case Tile::SliderInactive: return SpecialTileSpiteIds { .begin = 9,  .middle = 12, .end = 15 };
    case Tile::JumpActive    : return SpecialTileSpiteIds { .begin = 18, .middle = 21, .end = 24 };
    case Tile::JumpInactive  : return SpecialTileSpiteIds { .begin = 27, .middle = 30, .end = 33 };
    default                  : m_assert (false, "New cases not covered.");
    }
    return SpecialTileSpiteIds {};
}

Color getColorForMissing (map::InteractableTileType type)
{
    switch (type) {
    case Tile::SliderActive  : return Color { .r = 255, .g = 187, .b = 36,  .a = 255 };
    case Tile::SliderInactive: return Color { .r = 113, .g = 54,  .b = 0,   .a = 255 };
    case Tile::JumpActive    : return Color { .r = 14,  .g = 152, .b = 254, .a = 255 };
    case Tile::JumpInactive  : return Color { .r = 0,   .g = 34,  .b = 154, .a = 255 };
    default                  : m_assert (false, "New cases not covered.");
    }
    return RED;
}

void drawMissing (const world::LevelInteractableTiles& tiles)
{
    const auto& manager  = TextureManager::getInstance();
    const auto& tileSize = world::settings::tileSize;
    for (const auto& tile : tiles.getInteractableTiles()) {
        geom::Point2D pos (tile.begin.x * tileSize.width, (tile.begin.y - 1) * tileSize.height);
        const int nTiles = tile.end.x - tile.begin.x + 1;
        for (int i = 0; i < nTiles; i++) {
            manager.drawMissing (pos, tileSize.width, tileSize.height, getColorForMissing(tile.type));
            pos.x += tileSize.width;
        }
    }
}

}

void draw (const world::LevelInteractableTiles& tiles, const map::types::OptRefEmbeddedTileset& tilesetRef)
{
    if (!tilesetRef.has_value()) {
        drawMissing (tiles);
        return;
    }
    const auto& tileset  = tilesetRef.value().get();
    const auto& manager  = TextureManager::getInstance();
    const auto& tileSize = world::settings::tileSize;

    for (const auto& tile : tiles.getInteractableTiles()) {
        geom::Point2D pos (tile.begin.x * tileSize.width, //
                          (tile.begin.y - 1) * tileSize.height);
        const SpecialTileSpiteIds ids = getSpriteIds (tile.type);

        manager.drawTile (tileset.getImageID(), pos,       //
                          tileSize.width, tileSize.height, //
                          tileset.GetTilePositionById (ids.begin));

        const int nMidTiles = tile.end.x - tile.begin.x - 1;
        for (int i = 1; i < nMidTiles + 1; i++) {
            pos.x += tileSize.width;
            manager.drawTile (tileset.getImageID(), pos,       //
                              tileSize.width, tileSize.height, //
                              tileset.GetTilePositionById (ids.middle));
        }

        pos.x += tileSize.width;
        manager.drawTile (tileset.getImageID(), pos,       //
                          tileSize.width, tileSize.height, //
                          tileset.GetTilePositionById (ids.end));
    }
}

void draw (const world::LevelActuators& actuators, const map::CollectionTileset& tileset)
{
    const auto& manager = TextureManager::getInstance();
    for (const auto& actuator: actuators.getActuatorValues()) {
        if (tileset.isTileBelongsToSet(actuator.tileGid)) {
            manager.draw (tileset.getImageID(actuator.tileGid), actuator.pos);
        }
    }
}

} // namespace graphics
