#pragma once

#include <Animation/Animation.h>
#include <Animation/PlayerAnimation.h>
#include <Characters/PlayerAction.h>
#include <Characters/PlayerMovementMode.h>
#include <Physics/Collider.h>
#include <Physics/PhysicsUpdateState.h>

struct GroundData;

namespace PlayerMovement
{
struct State
{
    PlayerAction nextAction;
    player::MovementMode currentMode;
    float currentSimTime;
    phs::Vector2D m_velocity;
};
}

namespace player {
struct State {
    PlayerAction nextAction;
    MovementMode currentMode;
    ColliderMode currentColliderMode;
    float currentSimTime;
    phs::Vector2D m_velocity;
};
}

class Player
{
public:
    Player();
    Player& operator= (const Player& other) = default;

    virtual void Draw();
    virtual void Update (const player::UpdateState& updateState);

    void AddAction (PlayerAction action);

    const phs::Point2D& Pos() const { return m_pos; }
    phs::Point2D& Pos() { return m_pos; }

    const phs::Vector2D& Velocity() const { return m_velocity; }
    phs::Vector2D& Velocity() { return m_velocity; }

    phs::Collider getCollider() const;
    phs::Collider getCollider (player::ColliderMode mode) const;
    //PlayerMovement::State GetMovementState() const
    //{
    //    return { m_nextAction, m_currentMovementMode, m_currSimTime, m_velocity };
    //}

    player::State getState() const 
    {
        return {m_nextAction, m_currentMovementMode, m_currentColliderMode, m_currSimTime, m_velocity};
    }

private:
    void ChangeActiveMM (player::MovementMode newMode);

private:
    PlayerAction m_nextAction;
    player::MovementMode m_currentMovementMode;
    player::ColliderMode m_currentColliderMode;

    float m_currSimTime;

    phs::Vector2D m_velocity;
    phs::Point2D m_pos;

    //anim::PlayerAnimation m_procAnim;

    //vis part
    anim::PlayerSprite  m_currentSprite{anim::PlayerSprite::BodyCoreFull};
    uint8_t             m_currentFrame{0};
    float               m_lastPointerAngle{0.};
    float               m_lastThrusterAngle{0.};
    phs::Vector2D       m_lastPointerDir = phs::Vector2D (-1, 0);
};


namespace PlayerMovement
{
void MovePlayer (Player& player);
void JumpPlayer (Player& player);
}

