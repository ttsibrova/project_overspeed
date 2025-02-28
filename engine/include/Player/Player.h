#pragma once

#include <Animation/Animation.h>
#include <Animation/PlayerAnimation.h>
#include <Physics/Collider.h>
#include <Physics/PhysicsUpdateState.h>
#include <Player/PlayerBodyState.h>
#include <Player/PlayerPhysicalState.h>

struct GroundData;

namespace player {
enum class ColliderType : uint8_t
{
    UNDEFINED,
    INTERACTION,
    PHYSICAL
};

class Player
{
public:
    Player();
    Player& operator= (const Player& other) = default;

    void draw();
    void update (const physics::movement::UpdateState& updateState);
    void update (const player::BodyState& newBodyState);

    void addAction (player::Action action);

    const geom::Point2D& getPosition() const { return m_pos; }
    geom::Point2D&       getPosition() { return m_pos; }

    const geom::Vector2D& getVelocity() const { return velocity; }
    geom::Vector2D&       getVelocity() { return velocity; }

    physics::Collider getCollider() const;
    physics::Collider getCollider (player::ColliderType mode) const;

    player::PhysicalState getPhysicalState() const
    {
        return { m_nextAction, m_currentMovementMode, m_currSimTime, velocity };
    }

    player::BodyState getBodyState() const
    {
        return { .core = m_currentCoreSprite, .pointer = m_currentPointerSprite };
    }

private:
    void changeMovementMode (player::MovementMode newMode);

private:
    player::Action       m_nextAction;
    player::MovementMode m_currentMovementMode;

    float m_currSimTime;

    geom::Vector2D velocity;
    geom::Point2D  m_pos;

    // PlayerAnimation m_procAnim;

    // vis part
    player::CoreState    m_currentCoreSprite    = { player::CoreState::FULL };
    player::PointerState m_currentPointerSprite = { player::PointerState::SHARP };
    uint8_t              m_currentFrame         = { 0 };
    float                m_lastThrusterAngle    = { 0. };
    geom::Vector2D       m_lastPointerDir       = geom::Vector2D (-1, 0);
};

void MovePlayer (Player& player);
void JumpPlayer (Player& player);
} // namespace player
