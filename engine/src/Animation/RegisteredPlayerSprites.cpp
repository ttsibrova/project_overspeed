#include <Animation/RegisteredPlayerSprites.h>

#include <Graphics/TextureManager.h>


#include <cassert>

namespace anim
{



size_t GetTextureId (PlayerSprite sprite)
{
    switch (sprite)
    {
    case anim::PlayerSprite::BodyPointer:
        return 1001;
    case anim::PlayerSprite::BodyCoreEmpty:
        return 1002;
    case anim::PlayerSprite::BodyCoreFull:
        return 1003;
    case anim::PlayerSprite::BodyThruster:
        return 1004;
    default:
        assert (false); // add new cases
        break;
    }
    return 0;
}

SpriteInfo GetSpriteInfo (PlayerSprite sprite)
{
    switch (sprite)
    {
    case anim::PlayerSprite::BodyPointer:
        return SpriteInfo {.width = 45.f, .height = 43.f, .id = GetTextureId (sprite)};
    case anim::PlayerSprite::BodyCoreEmpty:
    case anim::PlayerSprite::BodyCoreFull:
        return SpriteInfo {.width = 43.f, .height = 42.f, .id = GetTextureId (sprite)};
    case anim::PlayerSprite::BodyThruster:
        return SpriteInfo {.width = 12.f, .height = 17.f, .id = GetTextureId (sprite)};
    default:
        break;
    }
    assert (false);
    return SpriteInfo{};
}

void RegisterPlayerSprites()
{
    auto& tManager = TextureManager::GetInstance();
    tManager.Load (GetTextureId (PlayerSprite::BodyPointer), "assets/overspeeder/pointer_glow_left.png");
    tManager.Load (GetTextureId (PlayerSprite::BodyCoreEmpty), "assets/overspeeder/core_empty.png");
    tManager.Load (GetTextureId (PlayerSprite::BodyCoreFull), "assets/overspeeder/core_full_glow.png");
    tManager.Load (GetTextureId (PlayerSprite::BodyThruster), "assets/overspeeder/thruster_glow.png");

}

}
