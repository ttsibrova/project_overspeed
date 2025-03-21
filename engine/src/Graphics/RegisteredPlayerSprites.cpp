#include <Graphics/RegisteredPlayerSprites.h>

#include <Graphics/TextureManager.h>

#include <cassert>

namespace graphics {

size_t getTextureId (PlayerSprite sprite)
{
    switch (sprite) {
    case PlayerSprite::BodyPointer:
        return 1001;
    case PlayerSprite::BodyCoreEmpty:
        return 1002;
    case PlayerSprite::BodyCoreFull:
        return 1003;
    case PlayerSprite::BodyThruster:
        return 1004;
    case PlayerSprite::BodyPointerShield:
        return 1005;
    default:
        assert (false); // add new cases
        break;
    }
    return 0;
}

SpriteInfo getSpriteInfo (PlayerSprite sprite)
{
    switch (sprite) {
    case PlayerSprite::BodyPointer:
        return SpriteInfo { .width = 25.f, .height = 23.f, .id = getTextureId (sprite) };
    case PlayerSprite::BodyCoreEmpty:
    case PlayerSprite::BodyCoreFull:
        return SpriteInfo { .width = 43.f, .height = 42.f, .id = getTextureId (sprite) };
    case PlayerSprite::BodyThruster:
        return SpriteInfo { .width = 12.f, .height = 17.f, .id = getTextureId (sprite) };
    case PlayerSprite::BodyPointerShield:
        return SpriteInfo { .width = 19.f, .height = 45.f, .id = getTextureId (sprite) };
    default:
        break;
    }
    assert (false);
    return SpriteInfo {};
}

void registerPlayerSprites()
{
    auto& tManager = TextureManager::getInstance();
    tManager.Load (getTextureId (PlayerSprite::BodyPointer), "assets/overspeeder/pointer_left.png");
    tManager.Load (getTextureId (PlayerSprite::BodyCoreEmpty), "assets/overspeeder/core_empty.png");
    tManager.Load (getTextureId (PlayerSprite::BodyCoreFull), "assets/overspeeder/core_v2.png");
    tManager.Load (getTextureId (PlayerSprite::BodyThruster), "assets/overspeeder/thruster_glow.png");
    tManager.Load (getTextureId (PlayerSprite::BodyPointerShield), "assets/overspeeder/shield_v2.png");
}

} // namespace graphics
