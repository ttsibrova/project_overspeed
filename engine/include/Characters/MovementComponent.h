#pragma once
#include <Characters/Character.h>

enum class EMovementMode
{
    IDLE,
    MOVEMENT,
    FALLING
};

class MovementComponent
{
public:
    MovementComponent (Character* owner);

    void Update (double dt);

public:
    inline void SetMaxMovingSpeed (float speed) { m_maxMovingSpeed = speed; }

public:
    inline bool IsMoving() { return m_bIsMoving; }

    void Move (const phs::Vector2D& dir);
    void ExitMoving();

    void Jump();

    void Falling();

private:
    void PhysMoving (double dt);

private:
    bool m_bIsMoving;
    bool m_bIsJumping;
    bool m_bIsFalling;
    bool m_bIsGrounded;

// Movement
    bool m_bCanMove;
    float m_maxMovingSpeed;
    phs::Vector2D m_MovingDirection;

private:
    Character* m_myOwner;
    EMovementMode m_currMM;
};
