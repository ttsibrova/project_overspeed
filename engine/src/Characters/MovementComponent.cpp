#include <Characters/MovementComponent.h>
#include <Characters/Player.h>

MovementComponent::MovementComponent (Player* owner):
    m_bIsFalling (false),
    m_bIsGrounded (false),
    m_bIsJumping (false),
    m_bIsMoving (false),
    m_bCanMove (false),
    m_maxMovingSpeed (200.f),
    m_myOwner (owner),
    m_currMM (EMovementMode::IDLE)
{
}

void MovementComponent::Update (double dt)
{
    switch (m_currMM)
    {
    case EMovementMode::IDLE:
        break;
    case EMovementMode::MOVEMENT:
    {
        if (m_bCanMove) {
            PhysMoving (dt);
            m_bCanMove = false;
        } else {
            ExitMoving();
        }
        break;
    }
    case EMovementMode::FALLING:
        break;
    default:
        break;
    }

}

void MovementComponent::Move (const phs::Vector2D& dir)
{
    if (m_bIsFalling || m_bIsJumping) {
        return;
    }
    m_currMM = EMovementMode::MOVEMENT;
    m_bCanMove = true;
    m_MovingDirection = dir;
}

void MovementComponent::ExitMoving()
{
    m_bIsMoving = false;
    m_MovingDirection.X() = 0;
    m_MovingDirection.Y() = 0;
    m_currMM = EMovementMode::IDLE;
}

void MovementComponent::PhysMoving (double dt)
{
    m_bIsMoving = true;

    //add check for float to double cast
    auto moveVec = m_MovingDirection * (m_maxMovingSpeed * static_cast <float> (dt));
    if (m_myOwner->ForwardDir().X() * moveVec.X() < 0) {
        m_myOwner->ForwardDir().FlipX();
    }
    m_myOwner->Pos().Translate (moveVec);
}
