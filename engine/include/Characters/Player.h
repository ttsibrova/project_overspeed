#pragma once

#include <Animation/Animation.h>
#include <Animation/PlayerAnimation.h>
#include <Characters/PlayerAction.h>
#include <Characters/PlayerMovementMode.h>
#include <Characters/PlayerStates.h>
#include <Physics/Collider.h>
#include <Physics/PhysicsUpdateState.h>

struct GroundData;


class Player
{
public:
    Player();
    Player& operator= (const Player& other) = default;

    void Draw();
    void Update (const PhysicsUpdateState& updateState);
    void Update (const player::BodyState& newBodyState);

    void AddAction (player::Action action);

    const phs::Point2D& Pos() const { return m_pos; }
    phs::Point2D& Pos() { return m_pos; }

    const phs::Vector2D& Velocity() const { return m_velocity; }
    phs::Vector2D& Velocity() { return m_velocity; }

    phs::Collider getCollider() const;
    phs::Collider getCollider (player::ColliderType mode) const;

    player::PhysicalState getPhysicalState() const 
    {
        return {m_nextAction, m_currentMovementMode, m_currSimTime, m_velocity};
    }

    player::BodyState getBodyState() const
    {
        return {.core = m_currentCoreSprite, .pointer = m_currentPointerSprite};
    }

private:
    void ChangeActiveMM (player::MovementMode newMode);

private:
    player::Action m_nextAction;
    player::MovementMode m_currentMovementMode;

    float m_currSimTime;

    phs::Vector2D m_velocity;
    phs::Point2D m_pos;

    //anim::PlayerAnimation m_procAnim;

    //vis part
    player::CoreState    m_currentCoreSprite{player::CoreState::EMPTY};
    player::PointerState m_currentPointerSprite{player::PointerState::SHARP};
    uint8_t              m_currentFrame{0};
    float                m_lastThrusterAngle{0.};
    phs::Vector2D        m_lastPointerDir = phs::Vector2D (-1, 0);
};


namespace PlayerMovement
{
void MovePlayer (Player& player);
void JumpPlayer (Player& player);
}

