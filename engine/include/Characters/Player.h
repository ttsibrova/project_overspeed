#pragma once

#include <Animation/Animation.h>
#include <Characters/PlayerAction.h>
#include <Characters/PlayerMovementMode.h>
#include <Physics/Collider.h>
#include <Physics/PhysicsUpdateState.h>

struct GroundData;

namespace PlayerMovement
{
struct PlayerMovementState
{
    PlayerAction nextAction;
    MovementMode currentMode;
    float currentSimTime;
    phs::Vector2D m_velocity;
};
}

struct PlayerProperties
{
    float height {1.f};
    float width {1.f};
};

class Player
{
public:
    Player (PlayerProperties props);
    Player& operator= (const Player& other) = default;

    virtual void Draw();
    virtual void Update (const PhysicsUpdateState& updateState);

    void AddAction (PlayerAction action);

    const phs::Point2D& Pos() const { return m_pos; }
    phs::Point2D& Pos() { return m_pos; }

    const phs::Trsf2D& Trsf() const { return m_trsf; }
    phs::Trsf2D& Trsf() { return m_trsf; }

    phs::Point2D TransformedPos() const { return m_pos.Translated (m_trsf.GetTranslationPart()); }

    const phs::Vector2D& Velocity() const { return m_velocity; }
    phs::Vector2D& Velocity() { return m_velocity; }

    Collider GetCollider() const { return Collider (m_pos, m_height, m_width); }
    PlayerMovement::PlayerMovementState GetMovementState() const { return {m_nextAction, m_currentMM, m_currSimTime, m_velocity};}

private:
    void ChangeActiveMM (MovementMode newMode);
    void FlipRendering();

private:
    RenderFlip   m_flip;
    PlayerAction m_nextAction;
    MovementMode m_currentMM;

    float m_currSimTime;
    float m_height;
    float m_width;

    phs::Vector2D m_velocity;
    phs::Point2D m_pos;
    phs::Trsf2D m_trsf;

    Animation         m_anim;
};


namespace PlayerMovement
{
void MovePlayer (Player& player);
void JumpPlayer (Player& player);
}

