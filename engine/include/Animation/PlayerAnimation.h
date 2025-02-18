#pragma once
#include <Geom/Point2D.h>
#include <Geom/Vector2D.h>
#include <Graphics/RegisteredPlayerSprites.h>
#include <Physics/Collider.h>

namespace anim {
class PlayerAnimation
{
public:
    void draw (const geom::Point2D& playerPos, const geom::Vector2D& playerVelocity);
    //void update();

    physics::Collider ComputeCurrentCollider (const geom::Point2D& playerPos) const;

private:
    graphics::PlayerSprite  m_currentCoreSprite{graphics::PlayerSprite::BodyCoreFull};
    uint8_t                 m_currentFrame{0};
    float                   m_lastPointerAngle{0.};
    float                   m_lastThrusterAngle {0.};
    geom::Vector2D          m_lastPointerDir = geom::Vector2D (-1, 0);
};
}
