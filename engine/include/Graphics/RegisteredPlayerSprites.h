#pragma once
#include <cstdint>

namespace graphics {

enum class PlayerSprite : uint8_t
{
    BodyPointer,
    BodyPointerShield,
    BodyCoreEmpty,
    BodyCoreFull,
    BodyThruster,
};

struct SpriteInfo
{
    float  width;
    float  height;
    size_t id;
};

size_t     getTextureId (const PlayerSprite sprite);
SpriteInfo getSpriteInfo (const PlayerSprite sprite);

void registerPlayerSprites();

} // namespace graphics
