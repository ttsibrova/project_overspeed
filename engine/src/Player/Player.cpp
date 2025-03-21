#include <Player/Player.h>

#include <Graphics/TextureManager.h>
#include <Input/InputGlobalGetters.h>
#include <raylib/raylib.h>

#include <cassert>
#include <print>
#include <type_traits>

#include <Debug/DebugLog.h>

namespace player {

namespace {

template <class BodyState>
graphics::PlayerSprite convertToSprite (BodyState state)
{
    if constexpr (std::is_same<BodyState, CoreState>()) {
        switch (state) {
        case CoreState::EMPTY:
            return graphics::PlayerSprite::BodyCoreEmpty;
        case CoreState::FULL:
            return graphics::PlayerSprite::BodyCoreFull;
        default:
            assert (false);
            return graphics::PlayerSprite::BodyCoreEmpty;
        }
    }
    if constexpr (std::is_same<BodyState, PointerState>()) {
        switch (state) {
        case PointerState::SHARP:
            return graphics::PlayerSprite::BodyPointer;
        case PointerState::SHIELD:
            return graphics::PlayerSprite::BodyPointerShield;
        default:
            assert (false);
            return graphics::PlayerSprite::BodyPointer;
        }
    }
}

} // anonymous namespace

Player::Player():
    m_nextAction (Action::IDLE),
    m_currentMovementMode (MovementMode::NONE),
    m_currSimTime (0.f)
{}

void Player::draw()
{
    // m_anim.draw (TransformedPos(), m_flip);
    // m_procAnim.draw (m_pos, velocity);

    const float pointerOffsetFromCore  = 30.f;
    const float shieldOffsetFromCore  = 22.f;
    const float thrusterOffsetFromCore = 22.f;

    auto  spriteInfo = getSpriteInfo (convertToSprite (m_currentCoreSprite));
    float halfWidth  = spriteInfo.width / 2.f;
    float halfHeight = spriteInfo.height / 2.f;

    geom::Point2D corePos { m_pos.X() - halfWidth, m_pos.Y() - halfHeight };

    float          rotationPointer  = 0.f;
    float          rotationThruster = 90.f;
    geom::Vector2D pointerOffset    = m_lastPointerDir;
    geom::Point2D  thrusterPos;

    if (velocity.SquareMagnitude() > geom::precision::quarter_pixel) {
        pointerOffset   = velocity.Normalized();
        float dot       = pointerOffset.Dot (geom::getLeftVector());
        rotationPointer = std::acos (dot) * 180 / PI;
        if (-pointerOffset.Y() < 0) { // special case for these calculations: Z component of cross product computation
                                      // when second vector is -1,0
            rotationPointer = -rotationPointer;
        }
        m_lastPointerDir = pointerOffset;
        thrusterPos      = m_pos.Translated (pointerOffset.Fipped() * thrusterOffsetFromCore);
        rotationThruster = rotationPointer;
    }
    else {
        geom::Vector2D inputVec = input::getAxisVec();
        if (inputVec.SquareMagnitude() > geom::precision::quarter_pixel) {
            m_lastPointerDir = inputVec.Normalized();
        }

        rotationPointer     = geom::computeAngleBetweenVectors (geom::getLeftVector(), m_lastPointerDir);
        float positiveAngle = std::abs (rotationPointer);
        if (positiveAngle > 0.5f && positiveAngle < 179.5f) {
            if (m_lastPointerDir.X() > 0) {
                rotationPointer  = 180.f;
                m_lastPointerDir = geom::getRightVector();
            }
            else {
                rotationPointer  = 0.f;
                m_lastPointerDir = geom::getLeftVector();
            }
        }
        thrusterPos = m_pos.Translated (geom::getDownVector() * thrusterOffsetFromCore);
    }

    geom::Vector2D playerInputVec = input::getAxisVec();
    if (playerInputVec.SquareMagnitude() > geom::precision::quarter_pixel) {
        auto vectorQuadrant = geom::getInversedQuadrant (geom::getVectorQudrant (playerInputVec));
        switch (vectorQuadrant) {
        case geom::Quadrant::I:
        case geom::Quadrant::II:
        case geom::Quadrant::III:
        case geom::Quadrant::IV:
            assert (false); // we should not be here
            break;
        case geom::Quadrant::X_ALIGNED:
            rotationThruster = 0;
            thrusterPos      = m_pos.Translated (geom::getRightVector() * thrusterOffsetFromCore);
            break;
        case geom::Quadrant::Y_ALIGNED:
            rotationThruster = 90.f;
            thrusterPos      = m_pos.Translated (geom::getDownVector() * thrusterOffsetFromCore);
            break;
        case geom::Quadrant::X_OPPOSITE:
            rotationThruster = 180.f;
            thrusterPos      = m_pos.Translated (geom::getLeftVector() * thrusterOffsetFromCore);
            break;
        case geom::Quadrant::Y_OPPOSITE:
            rotationThruster = -90.f;
            thrusterPos      = m_pos.Translated (geom::getUpVector() * thrusterOffsetFromCore);
            break;
        default:
            break;
        }
    }

    auto& textureManager = TextureManager::getInstance();
    textureManager.draw (spriteInfo.id, corePos);
    // std::print ("Rotation for pointer is {}\n", rotationPointer);

    geom::Point2D pointerPos = m_pos.Translated (pointerOffset * pointerOffsetFromCore);
    if (m_currentPointerSprite == PointerState::SHIELD) {
        pointerPos = m_pos.Translated (pointerOffset * shieldOffsetFromCore);
    }
    auto          pointerSpriteInfo = getSpriteInfo (convertToSprite (m_currentPointerSprite));
    geom::Point2D pointerOrigin (pointerSpriteInfo.width / 2.f, pointerSpriteInfo.height / 2.f);
    textureManager.DrawRotated (getTextureId (convertToSprite (m_currentPointerSprite)), pointerPos,
                                pointerSpriteInfo.width, pointerSpriteInfo.height, pointerOrigin, rotationPointer);

    auto          thrusterSpriteInfo = graphics::getSpriteInfo (graphics::PlayerSprite::BodyThruster);
    geom::Point2D thrusterOrigin (thrusterSpriteInfo.width / 2.f, thrusterSpriteInfo.height / 2.f);
    textureManager.DrawRotated (graphics::getTextureId (graphics::PlayerSprite::BodyThruster), thrusterPos,
                                thrusterSpriteInfo.width, thrusterSpriteInfo.height, thrusterOrigin, rotationThruster);
}

void Player::update (const physics::movement::UpdateState& newPhysState)
{
    // std::print ("player mode: {} \n", std::to_underlying (m_currentMM));
    if (newPhysState.nextMode != m_currentMovementMode) {
        changeMovementMode (newPhysState.nextMode);
    }

    m_currentMovementMode = newPhysState.nextMode;
    m_currSimTime         = newPhysState.simTime;
    velocity              = newPhysState.velocity;
    m_pos.Translate (newPhysState.trsl);

    m_pos.X() = std::round (m_pos.X()); // snapping character to the pixel grid
    m_pos.Y() = std::round (m_pos.Y()); // snapping character to the pixel grid

    m_nextAction = Action::IDLE;

    Debug::Log (std::move (getCollider()));
    Debug::Log (std::move (getCollider (ColliderType::INTERACTION)), RED);
    // Debug::Log (updateState, m_pos);
    //std::print ("Player pos: {}", m_pos);
}

void Player::update (const BodyState& newBodyState)
{
    m_currentCoreSprite    = newBodyState.core;
    m_currentPointerSprite = newBodyState.pointer;
}

void Player::addAction (Action action)
{
    if (std::to_underlying (action) > std::to_underlying (m_nextAction)) {
        m_nextAction = action;
    }
}

physics::Collider Player::getCollider() const
{
    return getCollider (ColliderType::PHYSICAL);
}

physics::Collider Player::getCollider (ColliderType mode) const
{
    // const float coreWidth = 23;
    // const float coreHeight = 22;
    const float pointerLength       = 26;
    const float groundHoverDistance = 15;
    const float shieldRadius        = 32;

    switch (mode) {
    case ColliderType::UNDEFINED:
    {
        assert (false); // should not be there
        return getCollider();
    }
    case ColliderType::INTERACTION:
    {
        float minX = m_pos.X() - shieldRadius;
        float minY = m_pos.Y() - shieldRadius; // adding hover distance above as well

        float maxX = minX + 2 * shieldRadius;
        float maxY = minY + 2 * shieldRadius + groundHoverDistance;

        geom::Point2D pointerEndPos = m_pos.Translated (m_lastPointerDir * (30 + pointerLength));

        minX = std::min (minX, pointerEndPos.X());
        minY = std::min (minY, pointerEndPos.Y());

        maxX = std::max (maxX, pointerEndPos.X());
        maxY = std::max (maxY, pointerEndPos.Y());

        return physics::Collider (geom::Point2D (minX, minY), geom::Point2D (maxX, maxY));
    }
    case ColliderType::PHYSICAL:
    {
        float minX = m_pos.X() - shieldRadius;
        float minY = m_pos.Y() - shieldRadius; // adding hover distance above as well
        return physics::Collider (geom::Point2D (minX, minY), 2 * shieldRadius + groundHoverDistance, 2 * shieldRadius);
    }
    }
    std::unreachable();
}

void Player::changeMovementMode (MovementMode newMode)
{
    switch (newMode) {
    case MovementMode::NONE:
    {
        // m_anim.SetProps ("mage_idle", 0, 8, m_height, m_width, 0.08f);
        break;
    }
    case MovementMode::RUNNING:
    {
        // m_anim.SetProps ("mage_run", 0, 8, m_height, m_width, 0.08f);
        break;
    }
    case MovementMode::AIR_MOVEMENT:
        // m_anim.SetProps ("mage_idle", 0, 8, m_height, m_width, 0.08f);
        break;
    default:
        break;
    }
    m_currentMovementMode = newMode;
}

void MovePlayer (Player& player)
{
    player.addAction (Action::MOVE);
}

void JumpPlayer (Player& player)
{
    player.addAction (Action::JUMP);
}

} // namespace player
