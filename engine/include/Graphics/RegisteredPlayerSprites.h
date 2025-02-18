#pragma once
#include <cstdint>

namespace graphics {

enum class PlayerSprite: uint8_t
{
    BodyPointer,
    BodyCoreEmpty,
    BodyCoreFull,
    BodyThruster,
    BodyPointerShield,
};

struct SpriteInfo {
    float width;
    float height;
    size_t id;
};

size_t getTextureId (PlayerSprite sprite);
SpriteInfo getSpriteInfo (PlayerSprite sprite);

void registerPlayerSprites();

}
