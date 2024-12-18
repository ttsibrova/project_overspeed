#pragma once

#include <Animation/Animation.h>
#include <Characters/Character.h>
#include <Characters/MovementComponent.h>
#include <Inputs/Command.h>
#include <Physics/RigidBody.h>


class Player
{
public:
    Player (Properties props);

    virtual void Draw();
    virtual void Update(double dt);

    void Move();
    void Jump();

    const phs::Vector2D& Pos() const { return m_pos; }
    phs::Vector2D& Pos() { return m_pos; }

    const phs::Trsf2D& Trsf() const { return m_trsf; }
    phs::Trsf2D& Trsf() { return m_trsf; }

    phs::Vector2D TransformedPos() const { return m_pos.Transformed (m_trsf); }

    const phs::Vector2D& Acceleration() const { return m_acceleration; }
    phs::Vector2D& Acceleration() { return m_acceleration; }

    const phs::Vector2D& Velocity() const { return m_velocity; }
    phs::Vector2D& Velocity() { return m_velocity; }

    const phs::Vector2D& ForwardDir() const { return m_forwardDir; }
    phs::Vector2D& ForwardDir() { return m_forwardDir; }

private:
    phs::Vector2D m_pos;
    phs::Trsf2D m_trsf;

    phs::Vector2D m_acceleration;
    phs::Vector2D m_velocity;
    phs::Vector2D m_forwardDir;


    Animation         m_anim;
    RenderFlip        m_flip;
    MovementComponent m_mc;

    float m_height;
    float m_width;
};


namespace PlayerMovement
{
void MovePlayer (Player& player);

}

