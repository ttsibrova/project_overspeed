#pragma once
#include <cstdint>

namespace anim {

enum class PlayerSprite: uint8_t
{
    BodyPointer,
    BodyCoreEmpty,
    BodyCoreFull,
    BodyThruster
};

struct SpriteInfo {
    float width;
    float height;
    size_t id;
};

size_t GetTextureId (PlayerSprite sprite);
SpriteInfo GetSpriteInfo (PlayerSprite sprite);

void RegisterPlayerSprites();

}
