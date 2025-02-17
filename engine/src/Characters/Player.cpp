#include <Characters/Player.h>

#include <Characters/PlayerMovement.h>
#include <Graphics/TextureManager.h>
#include <Inputs/InputHandler.h> 
#include <raylib/raylib.h>

#include <cassert>
#include <print>
#include <type_traits>

#include <Debug/DebugLog.h>


template <class BodyState>
anim::PlayerSprite convertToSprite (BodyState state)
{
    if constexpr (std::is_same <BodyState, player::CoreState>()) {
        switch (state)
        {
        case player::CoreState::EMPTY: return anim::PlayerSprite::BodyCoreEmpty;
        case player::CoreState::FULL: return anim::PlayerSprite::BodyCoreFull;
        default:
            assert (false);
            return anim::PlayerSprite::BodyCoreEmpty;
        }
    }
    if constexpr (std::is_same <BodyState, player::PointerState>()) {
        switch (state)
        {
        case player::PointerState::SHARP: return anim::PlayerSprite::BodyPointer;
        case player::PointerState::SHIELD: return anim::PlayerSprite::BodyPointerShield;
        default:
            assert (false);
            return anim::PlayerSprite::BodyPointer;
        }
    }
}

Player::Player():
    m_nextAction (player::Action::IDLE),
    m_currentMovementMode (player::MovementMode::NONE),
    m_currSimTime (0.f)
{
}

void Player::Draw()
{
    //m_anim.Draw (TransformedPos(), m_flip);
    //m_procAnim.Draw (m_pos, m_velocity);

    const float pointerOffsetFromCore = 30.f;
    const float thrusterOffsetFromCore = 22.f;

    auto spriteInfo = anim::GetSpriteInfo (convertToSprite (m_currentCoreSprite));
    float halfWidth = spriteInfo.width / 2.f;
    float halfHeight = spriteInfo.height / 2.f;

    phs::Point2D corePos{m_pos.X() - halfWidth, m_pos.Y() - halfHeight};

    float rotationPointer = 0.f;
    float rotationThruster = 90.f;
    phs::Vector2D pointerOffset = m_lastPointerDir;
    phs::Point2D thrusterPos;

    if (m_velocity.SquareMagnitude() > phs::Precision::quarter_pixel) {
        pointerOffset = m_velocity.Normalized();
        float dot = pointerOffset.Dot (phs::GetLeftVector());
        rotationPointer = std::acos (dot) * 180 / PI;
        if (-pointerOffset.Y() < 0) { //special case for these calculations: Z component of cross product computation when second vector is -1,0
            rotationPointer = -rotationPointer;
        }
        m_lastPointerDir = pointerOffset;
        thrusterPos = m_pos.Translated (pointerOffset.Fipped() * thrusterOffsetFromCore);
        rotationThruster = rotationPointer;
    }
    else {
        phs::Vector2D inputVec = Input::GetAxisVec();
        if (inputVec.SquareMagnitude() > phs::Precision::quarter_pixel) {
            m_lastPointerDir = inputVec.Normalized();
        }

        rotationPointer = phs::computeAngleBetweenVectors (phs::GetLeftVector(), m_lastPointerDir);
        float positiveAngle = std::abs (rotationPointer);
        if (positiveAngle > 0.5f && positiveAngle < 179.5f) {
            if (m_lastPointerDir.X() > 0) {
                rotationPointer = 180.f;
                m_lastPointerDir = phs::GetRightVector();
            }
            else {
                rotationPointer = 0.f;
                m_lastPointerDir = phs::GetLeftVector();
            }
        }
        thrusterPos = m_pos.Translated (phs::GetDownVector() * thrusterOffsetFromCore);
    }

    phs::Vector2D playerInputVec = Input::GetAxisVec();
    if (playerInputVec.SquareMagnitude() > phs::Precision::quarter_pixel) {
        auto vectorQuadrant = phs::GetInversedQuadrant (phs::GetVectorQudrant (playerInputVec));
        switch (vectorQuadrant)
        {
        case phs::Quadrant::I:
        case phs::Quadrant::II:
        case phs::Quadrant::III:
        case phs::Quadrant::IV:
            assert (false); // we should not be here
            break;
        case phs::Quadrant::X_ALIGNED:
            rotationThruster = 0;
            thrusterPos = m_pos.Translated (phs::GetRightVector() * thrusterOffsetFromCore);
            break;
        case phs::Quadrant::Y_ALIGNED:
            rotationThruster = 90.f;
            thrusterPos = m_pos.Translated (phs::GetDownVector() * thrusterOffsetFromCore);
            break;
        case phs::Quadrant::X_OPPOSITE:
            rotationThruster = 180.f;
            thrusterPos = m_pos.Translated (phs::GetLeftVector() * thrusterOffsetFromCore);
            break;
        case phs::Quadrant::Y_OPPOSITE:
            rotationThruster = -90.f;
            thrusterPos = m_pos.Translated (phs::GetUpVector() * thrusterOffsetFromCore);
            break;
        default:
            break;
        }
    }

    auto& textureManager = TextureManager::GetInstance();
    textureManager.Draw (spriteInfo.id, corePos);
    //std::print ("Rotation for pointer is {}\n", rotationPointer);

    phs::Point2D pointerPos = m_pos;
    if (m_currentPointerSprite == player::PointerState::SHARP) {
        pointerPos = m_pos.Translated (pointerOffset * pointerOffsetFromCore);
    }
    auto pointerSpriteInfo = anim::GetSpriteInfo (convertToSprite(m_currentPointerSprite));
    phs::Point2D pointerOrigin (pointerSpriteInfo.width / 2.f, pointerSpriteInfo.height / 2.f);
    textureManager.DrawRotated (anim::GetTextureId (convertToSprite(m_currentPointerSprite)), pointerPos, pointerSpriteInfo.width, pointerSpriteInfo.height, pointerOrigin, rotationPointer);

    auto thrusterSpriteInfo = anim::GetSpriteInfo (anim::PlayerSprite::BodyThruster);
    phs::Point2D thrusterOrigin (thrusterSpriteInfo.width / 2.f, thrusterSpriteInfo.height / 2.f);
    textureManager.DrawRotated (anim::GetTextureId (anim::PlayerSprite::BodyThruster), thrusterPos, thrusterSpriteInfo.width, thrusterSpriteInfo.height, thrusterOrigin, rotationThruster);
}

void Player::Update (const PhysicsUpdateState& newPhysState)
{
    //std::print ("player mode: {} \n", std::to_underlying (m_currentMM));
    if (newPhysState.nextMode != m_currentMovementMode) {
        ChangeActiveMM (newPhysState.nextMode);
    }

    m_currentMovementMode = newPhysState.nextMode;
    m_currSimTime = newPhysState.simTime;
    m_velocity = newPhysState.velocity;
    m_pos.Translate (newPhysState.trsl);

    m_pos.X() = std::round (m_pos.X()); //snapping character to the pixel grid
    m_pos.Y() = std::round (m_pos.Y()); //snapping character to the pixel grid

    m_nextAction = player::Action::IDLE;

    Debug::Log (std::move (getCollider()));
    Debug::Log (std::move (getCollider (player::ColliderType::INTERACTION)), RED);
    //Debug::Log (updateState, m_pos);
}

void Player::Update (const player::BodyState& newBodyState)
{
    m_currentCoreSprite = newBodyState.core;
    m_currentPointerSprite = newBodyState.pointer;
}

void Player::AddAction (player::Action action)
{
    if (std::to_underlying (action) > std::to_underlying (m_nextAction)) {
        m_nextAction = action;
    }
}

phs::Collider Player::getCollider() const
{
    return getCollider (player::ColliderType::PHYSICAL);
}

phs::Collider Player::getCollider (player::ColliderType mode) const
{
    //const float coreWidth = 23;
    //const float coreHeight = 22;
    const float pointerLength = 26;
    const float groundHoverDistance = 15;
    const float shieldRadius = 29;

    switch (mode)
    {
    case player::ColliderType::UNDEFINED:
    {
        assert (false); // should not be there
        return getCollider();
    }
    case player::ColliderType::INTERACTION:
    {
        float minX = m_pos.X() - shieldRadius;
        float minY = m_pos.Y() - shieldRadius; // adding hover distance above as well

        float maxX = minX + 2 * shieldRadius;
        float maxY = minY + 2 * shieldRadius + groundHoverDistance;

        phs::Point2D pointerEndPos = m_pos.Translated (m_lastPointerDir * (30 + pointerLength));

        minX = std::min (minX, pointerEndPos.X());
        minY = std::min (minY, pointerEndPos.Y());

        maxX = std::max (maxX, pointerEndPos.X());
        maxY = std::max (maxY, pointerEndPos.Y());

        return phs::Collider (phs::Point2D (minX, minY), phs::Point2D (maxX, maxY));
    }
    case player::ColliderType::PHYSICAL:
    {
        float minX = m_pos.X() - shieldRadius;
        float minY = m_pos.Y() - shieldRadius; // adding hover distance above as well
        return phs::Collider (phs::Point2D (minX, minY), 2 * shieldRadius + groundHoverDistance, 2 * shieldRadius);
    }
    }
    std::unreachable();
}

void Player::ChangeActiveMM (player::MovementMode newMode)
{
    switch (newMode)
    {
    case player::MovementMode::NONE:
    {
        //m_anim.SetProps ("mage_idle", 0, 8, m_height, m_width, 0.08f);
        break;
    }
    case player::MovementMode::RUNNING:
    {
        //m_anim.SetProps ("mage_run", 0, 8, m_height, m_width, 0.08f);
        break;
    }
    case player::MovementMode::AIR_MOVEMENT:
        //m_anim.SetProps ("mage_idle", 0, 8, m_height, m_width, 0.08f);
        break;
    default:
        break;
    }
    m_currentMovementMode = newMode;
}

void PlayerMovement::MovePlayer (Player& player)
{
    player.AddAction (player::Action::MOVE);
}

void PlayerMovement::JumpPlayer (Player& player)
{
    player.AddAction (player::Action::JUMP);
}
