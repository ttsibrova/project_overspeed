#pragma once
#include <Animation/RegisteredPlayerSprites.h>
#include <Physics/Collider.h>
#include <Physics/Point2D.h>
#include <Physics/Vector2D.h>

namespace anim {
class PlayerAnimation
{
public:
    void Draw (const phs::Point2D& playerPos, const phs::Vector2D& playerVelocity);
    //void Update();

    phs::Collider ComputeCurrentCollider (const phs::Point2D& playerPos) const;

private:
    PlayerSprite  m_currentSprite{PlayerSprite::BodyCoreFull};
    uint8_t       m_currentFrame{0};
    float         m_lastPointerAngle{0.};
    float         m_lastThrusterAngle {0.};
    phs::Vector2D m_lastPointerDir = phs::Vector2D (-1, 0);
};
}
