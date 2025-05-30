#include <Animation/PlayerAnimation.h>

#include <Geom/Point2D.h>
#include <Graphics/RegisteredPlayerSprites.h>
#include <Graphics/TextureManager.h>
#include <Input/InputGlobalGetters.h>

#include <cassert>
#include <cmath>

namespace anim {

void PlayerAnimation::draw (const geom::Point2D& playerPos, const geom::Vector2D& playerVelocity)
{
    const float pointerOffsetFromCore  = 30.f;
    const float thrusterOffsetFromCore = 22.f;

    auto  spriteInfo = getSpriteInfo (m_currentCoreSprite);
    float halfWidth  = spriteInfo.width / 2.f;
    float halfHeight = spriteInfo.height / 2.f;

    geom::Point2D corePos { playerPos.x - halfWidth, playerPos.y - halfHeight };

    float          rotationPointer  = m_lastPointerAngle;
    float          rotationThruster = 90.f;
    geom::Vector2D pointerOffset    = m_lastPointerDir;
    geom::Point2D  thrusterPos;

    if (playerVelocity.getSquareMagnitude() > geom::precision::quarterPixel) {
        pointerOffset   = playerVelocity.normalized();
        float dot       = pointerOffset.dot (geom::getLeftVector());
        rotationPointer = std::acos (dot) * 180 / PI;
        // special case for these calculations: Z component of cross product computation when second vector is -1,0
        if (-pointerOffset.y < 0) {
            rotationPointer = -rotationPointer;
        }
        m_lastPointerAngle = rotationPointer;
        m_lastPointerDir   = pointerOffset;
        thrusterPos        = playerPos.translated (pointerOffset.fipped() * thrusterOffsetFromCore);
        rotationThruster   = rotationPointer;
    }
    else {
        float positiveAngle = std::abs (m_lastPointerAngle);
        if (positiveAngle > 0.5f && positiveAngle < 179.5f) {
            if (m_lastPointerDir.x > 0) {
                m_lastPointerAngle = 180.f;
                m_lastPointerDir   = geom::getRightVector();
            }
            else {
                m_lastPointerAngle = 0.f;
                m_lastPointerDir   = geom::getLeftVector();
            }
        }
        thrusterPos = playerPos.translated (geom::getDownVector() * thrusterOffsetFromCore);
    }

    geom::Vector2D playerInputVec = input::getAxisVec();
    if (playerInputVec.getSquareMagnitude() > geom::precision::quarterPixel) {
        auto vectorQuadrant = geom::getInversedQuadrant (geom::getVectorQudrant (playerInputVec));
        switch (vectorQuadrant) {
        case geom::Quadrant::I:
        case geom::Quadrant::II:
        case geom::Quadrant::III:
        case geom::Quadrant::IV:
            assert (false); // we should not be here
            break;
        case geom::Quadrant::X_ALIGNED:
            rotationThruster = 0;
            thrusterPos      = playerPos.translated (geom::getRightVector() * thrusterOffsetFromCore);
            break;
        case geom::Quadrant::Y_ALIGNED:
            rotationThruster = 90.f;
            thrusterPos      = playerPos.translated (geom::getDownVector() * thrusterOffsetFromCore);
            break;
        case geom::Quadrant::X_OPPOSITE:
            rotationThruster = 180.f;
            thrusterPos      = playerPos.translated (geom::getLeftVector() * thrusterOffsetFromCore);
            break;
        case geom::Quadrant::Y_OPPOSITE:
            rotationThruster = -90.f;
            thrusterPos      = playerPos.translated (geom::getUpVector() * thrusterOffsetFromCore);
            break;
        default:
            break;
        }
    }

    geom::Point2D pointerPos = playerPos.translated (pointerOffset * pointerOffsetFromCore);

    auto& textureManager = TextureManager::getInstance();
    textureManager.draw (spriteInfo.id, corePos);
    // std::print ("Rotation for pointer is {}\n", rotationPointer);
    auto          pointerSpriteInfo = graphics::getSpriteInfo (graphics::PlayerSprite::BodyPointer);
    geom::Point2D pointerOrigin (pointerSpriteInfo.width / 2.f, pointerSpriteInfo.height / 2.f);
    textureManager.drawRotated (graphics::getTextureId (graphics::PlayerSprite::BodyPointer), pointerPos,
                                pointerSpriteInfo.width, pointerSpriteInfo.height, pointerOrigin, rotationPointer);

    auto          thrusterSpriteInfo = graphics::getSpriteInfo (graphics::PlayerSprite::BodyThruster);
    geom::Point2D thrusterOrigin (thrusterSpriteInfo.width / 2.f, thrusterSpriteInfo.height / 2.f);
    textureManager.drawRotated (graphics::getTextureId (graphics::PlayerSprite::BodyThruster), thrusterPos,
                                thrusterSpriteInfo.width, thrusterSpriteInfo.height, thrusterOrigin, rotationThruster);
}

physics::Collider PlayerAnimation::ComputeCurrentCollider (const geom::Point2D& playerPos) const
{
    const float coreWidth           = 23.f;
    const float coreHeight          = 22.f;
    const float pointerLength       = 26.f;
    const float groundHoverDistance = 45.f;

    float minX = playerPos.x - coreWidth / 2.f;
    float minY = playerPos.y - coreHeight / 2.f - groundHoverDistance; // adding hover distance above as well

    float maxX = minX + coreWidth;
    float maxY = minY + coreHeight + 2 * groundHoverDistance;

    geom::Point2D pointerEndPos = playerPos.translated (m_lastPointerDir * (30 + pointerLength));

    minX = std::min (minX, pointerEndPos.x);
    minY = std::min (minY, pointerEndPos.y);

    maxX = std::max (maxX, pointerEndPos.x);
    maxY = std::max (maxY, pointerEndPos.y);

    return physics::Collider (geom::Point2D (minX, minY), geom::Point2D (maxX, maxY));
}

} // namespace anim
