#include <Characters/Player.h>

#include <Characters/PlayerMovement.h>
#include <Graphics/TextureManager.h>
#include <Inputs/InputHandler.h> 
#include <raylib/raylib.h>

#include <print>

Player::Player (Properties props):
    m_flip (FLIP_NONE),
    m_nextAction (PlayerAction::IDLE),
    m_currentMM (MovementMode::NONE),
    m_currSimTime (0.f),
    m_height (props.Height),
    m_width (props.Width)
{
    m_anim.SetProps ("mage_idle", 0, 8, m_height, m_width, 0.08f);
}

void Player::Draw()
{
    m_anim.Draw (TransformedPos(), m_flip);
}

void Player::Update (const PhysicsUpdateState& updateState)
{
    std::print ("player mode: {} \n", std::to_underlying (m_currentMM));
    if (updateState.nextMode != m_currentMM) {
        ChangeActiveMM (updateState.nextMode);
    }

    m_currentMM = updateState.nextMode;
    m_currSimTime = updateState.simTime;
    m_velocity = updateState.velocity;
    m_pos.Translate (updateState.trsf.GetTranslationPart());

    if (m_velocity.SquareMagnitude() > 1.e-7f) {
        if (phs::IsOpposite (m_velocity, phs::Vector2D (1.f, 0.f))) {
            m_flip = FLIP_HORIZONTAL;
        } else {
            m_flip = FLIP_NONE;
        }
    }

    m_anim.Update();
    m_nextAction = PlayerAction::IDLE;
}

void Player::AddAction (PlayerAction action)
{
    if (std::to_underlying (action) > std::to_underlying (m_nextAction)) {
        m_nextAction = action;
    }
}

void Player::ChangeActiveMM (MovementMode newMode)
{
    m_currSimTime = 0.f;
    switch (newMode)
    {
    case MovementMode::NONE:
    {
        m_anim.SetProps ("mage_idle", 0, 8, m_height, m_width, 0.08f);
        m_velocity = phs::Vector2D();
        break;
    }
    case MovementMode::RUNNING:
    {
        m_anim.SetProps ("mage_run", 0, 8, m_height, m_width, 0.08f);
        break;
    }
    case MovementMode::FALLING:
        m_anim.SetProps ("mage_idle", 0, 8, m_height, m_width, 0.08f);
        break;
    default:
        break;
    }
    m_currentMM = newMode;
}

void Player::FlipRendering()
{
    if (m_flip == FLIP_NONE) {
        m_flip = FLIP_HORIZONTAL;
    }
    if (m_flip == FLIP_HORIZONTAL) {
        m_flip = FLIP_NONE;
    }
}

void PlayerMovement::MovePlayer (Player& player)
{
    player.AddAction (PlayerAction::MOVE);
}
