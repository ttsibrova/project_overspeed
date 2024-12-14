#include <Characters/Player.h>

#include <Graphics/TextureManager.h>
#include <Inputs/InputHandler.h> 
#include <raylib/raylib.h>


Player::Player (Properties props): Character (props),
    m_mc (this),
    m_height (props.Height),
    m_width (props.Width),
    m_flip (FLIP_NONE)
{
    m_anim.SetProps ("mage_run", 0, 8, m_height, m_width, 0.08f);
    m_mc.SetMaxMovingSpeed (250.f);
}

void Player::Draw()
{
    m_anim.Draw (TransformedPos(), m_flip);
}

void Player::Update (double dt)
{
    m_mc.Update (dt);
    if (m_mc.IsMoving()) {
        m_anim.SetProps ("mage_run", 0, 8, m_height, m_width, 0.08f);
    } else {
        m_anim.SetProps ("mage_idle", 0, 8, m_height, m_width, 0.16f);
    }

    if (ForwardDir().X() < 0) {
        m_flip = FLIP_HORIZONTAL;
    } else {
        m_flip = FLIP_NONE;
    }
    m_anim.Update();
}

void Player::Move()
{
    auto dir = InputHandler::GlobalInstance().GetAxisVec();
    phs::Log (dir, "Input axis");

    if (dir.SquareMagnitude() < 1e-5 || std::abs (dir.X()) < 1e-5) {
        return; //no direction
    }
    auto xDir = StripByAxis (dir, phs::EAxis::X);
    xDir.Normalize();
    m_mc.Move (xDir);
}

void Player::Jump()
{
}

bool MageMoveCommand::Execute (IObject* obj)
{
    if (auto player = dynamic_cast <Player*> (obj)) {
        player->Move();
        return true;
    }
    return false;
}

bool MageJumpCommand::Execute (IObject* obj)
{
    if (auto player = dynamic_cast <Player*> (obj)) {
        player->Jump();
        return true;
    }
    return false;
}
