#include <Animation/PlayerAnimation.h>

#include <Animation/RegisteredPlayerSprites.h>
#include <Graphics/TextureManager.h>
#include <Inputs/InputHandler.h>
#include <Physics/Point2D.h>

#include <cassert>
#include <cmath>
#include <print>

namespace anim {
void PlayerAnimation::Draw (const phs::Point2D& playerPos, const phs::Vector2D& playerVelocity)
{
    const float pointerOffsetFromCore = 30.f;
    const float thrusterOffsetFromCore = 22.f;

    auto spriteInfo = anim::GetSpriteInfo (m_currentSprite);
    float halfWidth = spriteInfo.width / 2.f;
    float halfHeight = spriteInfo.height / 2.f;

    phs::Point2D corePos {playerPos.X() - halfWidth, playerPos.Y() - halfHeight};

    float rotationPointer = m_lastPointerAngle;
    float rotationThruster = 90.f;
    phs::Vector2D pointerOffset = m_lastPointerDir;
    phs::Point2D thrusterPos;

    if (playerVelocity.SquareMagnitude() > phs::Precision::quarter_pixel) {
        pointerOffset = playerVelocity.Normalized();
        float dot = pointerOffset.Dot (phs::GetLeftVector());
        rotationPointer = std::acos (dot) * 180 / PI;
        if (-pointerOffset.Y() < 0) { //special case for these calculations: Z component of cross product computation when second vector is -1,0
            rotationPointer = -rotationPointer;
        }
        m_lastPointerAngle = rotationPointer;
        m_lastPointerDir = pointerOffset;
        thrusterPos = playerPos.Translated (pointerOffset.Fipped() * thrusterOffsetFromCore);
        rotationThruster = rotationPointer;
    }
    else {
        float positiveAngle = std::abs (m_lastPointerAngle);
        if (positiveAngle > 0.5f && positiveAngle < 179.5f) {
            if (m_lastPointerDir.X() > 0) {
                m_lastPointerAngle = 180.f;
                m_lastPointerDir = phs::GetRightVector();
            } else {
                m_lastPointerAngle = 0.f;
                m_lastPointerDir = phs::GetLeftVector();
            }
        }
        thrusterPos = playerPos.Translated (phs::GetDownVector() * thrusterOffsetFromCore);
    }

    phs::Vector2D playerInputVec = Input::GetAxisVec();
    if (playerInputVec.SquareMagnitude() > phs::Precision::quarter_pixel) {
        auto vectorQuadrant = phs::GetInversedQuadrant (phs::GetVectorQudrant (playerInputVec));
        switch (vectorQuadrant)
        {
        case phs::Quadrant::I:
        case phs::Quadrant::II:
        case phs::Quadrant::III:
        case phs::Quadrant::IV:
            assert (false); // we should not be here
            break;
        case phs::Quadrant::X_ALIGNED:
            rotationThruster = 0;
            thrusterPos = playerPos.Translated (phs::GetRightVector() * thrusterOffsetFromCore);
            break;
        case phs::Quadrant::Y_ALIGNED:
            rotationThruster = 90.f;
            thrusterPos = playerPos.Translated (phs::GetDownVector() * thrusterOffsetFromCore);
            break;
        case phs::Quadrant::X_OPPOSITE:
            rotationThruster = 180.f;
            thrusterPos = playerPos.Translated (phs::GetLeftVector() * thrusterOffsetFromCore);
            break;
        case phs::Quadrant::Y_OPPOSITE:
            rotationThruster = -90.f;
            thrusterPos = playerPos.Translated (phs::GetUpVector() * thrusterOffsetFromCore);
            break;
        default:
            break;
        }
    }

    phs::Point2D pointerPos = playerPos.Translated (pointerOffset * pointerOffsetFromCore);

    auto& textureManager = TextureManager::GetInstance();
    textureManager.Draw (spriteInfo.id, corePos);
    //std::print ("Rotation for pointer is {}\n", rotationPointer);
    auto pointerSpriteInfo = anim::GetSpriteInfo (anim::PlayerSprite::BodyPointer);
    phs::Point2D pointerOrigin (pointerSpriteInfo.width / 2.f, pointerSpriteInfo.height / 2.f);
    textureManager.DrawRotated (anim::GetTextureId (anim::PlayerSprite::BodyPointer), pointerPos, pointerSpriteInfo.width, pointerSpriteInfo.height, pointerOrigin, rotationPointer);

    auto thrusterSpriteInfo = anim::GetSpriteInfo (anim::PlayerSprite::BodyThruster);
    phs::Point2D thrusterOrigin (thrusterSpriteInfo.width / 2.f, thrusterSpriteInfo.height / 2.f);
    textureManager.DrawRotated (anim::GetTextureId (anim::PlayerSprite::BodyThruster), thrusterPos, thrusterSpriteInfo.width, thrusterSpriteInfo.height, thrusterOrigin, rotationThruster);
}


phs::Collider PlayerAnimation::ComputeCurrentCollider (const phs::Point2D& playerPos) const
{
    const float coreWidth = 23;
    const float coreHeight = 22;
    const float pointerLength = 26;
    const float groundHoverDistance = 45;

    float minX = playerPos.X() - coreWidth / 2.f;
    float minY = playerPos.Y() - coreHeight / 2.f - groundHoverDistance; // adding hover distance above as well

    float maxX = minX + coreWidth;
    float maxY = minY + coreHeight + 2 * groundHoverDistance;

    phs::Point2D pointerEndPos = playerPos.Translated (m_lastPointerDir * (30 + pointerLength));

    minX = std::min (minX, pointerEndPos.X());
    minY = std::min (minY, pointerEndPos.Y());

    maxX = std::max (maxX, pointerEndPos.X());
    maxY = std::max (maxY, pointerEndPos.Y());

    return phs::Collider (phs::Point2D (minX, minY), phs::Point2D (maxX, maxY));
}
}
