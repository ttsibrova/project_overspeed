#pragma once
#include <World/LevelInteractableTiles.h>
#include <World/LevelActuators.h>
#include <Map/Tileset.h>

namespace graphics
{

void draw (const world::LevelInteractableTiles& tiles, const map::types::OptRefEmbeddedTileset& tileset);
void draw (const world::LevelActuators& actuators, const map::CollectionTileset& tileset);

}
