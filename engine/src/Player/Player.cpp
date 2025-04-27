#include <Player/Player.h>

#include <Core/Logger.h>
#include <Graphics/TextureManager.h>
#include <Input/InputGlobalGetters.h>
#include <raylib.h>

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
        case CoreState::Empty:
            return graphics::PlayerSprite::BodyCoreEmpty;
        case CoreState::Full:
            return graphics::PlayerSprite::BodyCoreFull;
        default:
            assert (false);
            return graphics::PlayerSprite::BodyCoreEmpty;
        }
    }
    if constexpr (std::is_same<BodyState, PointerState>()) {
        switch (state) {
        case PointerState::Sharp:
            return graphics::PlayerSprite::BodyPointer;
        case PointerState::Shield:
            return graphics::PlayerSprite::BodyPointerShield;
        default:
            assert (false);
            return graphics::PlayerSprite::BodyPointer;
        }
    }
}

} // anonymous namespace

Player::Player():
    m_nextAction (Action::Idle),
    m_currentMovementMode (MovementMode::None),
    m_currSimTime (0.f)
{
    m_pos.x = 300.f;
    m_inputLayer.addAction (GAMEPAD_BUTTON_UNKNOWN, KEY_A, input::ActionType::Hold, [player = this](){player::MovePlayer(*player);});
    // m_inputLayer.addAction (GAMEPAD_BUTTON_UNKNOWN, KEY_W, input::ActionType::Hold, PlayerMovement::MovePlayer);
    // m_inputLayer.addAction (GAMEPAD_BUTTON_UNKNOWN, KEY_S, input::ActionType::Hold, PlayerMovement::MovePlayer);
    m_inputLayer.addAction (GAMEPAD_BUTTON_UNKNOWN, KEY_D, input::ActionType::Hold, [player = this](){player::MovePlayer(*player);});
    m_inputLayer.addAction (GAMEPAD_BUTTON_UNKNOWN, KEY_SPACE, input::ActionType::Press, [player = this](){player::JumpPlayer(*player);});
}

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

    geom::Point2D corePos { m_pos.x - halfWidth, m_pos.y - halfHeight };

    float          rotationPointer  = 0.f;
    float          rotationThruster = 90.f;
    geom::Vector2D pointerOffset    = m_lastPointerDir;
    geom::Point2D  thrusterPos;

    if (velocity.getSquareMagnitude() > geom::precision::quarterPixel) {
        pointerOffset   = velocity.normalized();
        float dot       = pointerOffset.dot (geom::getLeftVector());
        rotationPointer = std::acos (dot) * 180 / PI;
        if (-pointerOffset.y < 0) { // special case for these calculations: Z component of cross product computation
                                      // when second vector is -1,0
            rotationPointer = -rotationPointer;
        }
        m_lastPointerDir = pointerOffset;
        thrusterPos      = m_pos.translated (pointerOffset.fipped() * thrusterOffsetFromCore);
        rotationThruster = rotationPointer;
    }
    else {
        geom::Vector2D inputVec = input::getAxisVec();
        if (inputVec.getSquareMagnitude() > geom::precision::quarterPixel) {
            m_lastPointerDir = inputVec.normalized();
        }

        rotationPointer     = geom::computeAngleBetweenVectors (geom::getLeftVector(), m_lastPointerDir);
        float positiveAngle = std::abs (rotationPointer);
        if (positiveAngle > 0.5f && positiveAngle < 179.5f) {
            if (m_lastPointerDir.x > 0) {
                rotationPointer  = 180.f;
                m_lastPointerDir = geom::getRightVector();
            }
            else {
                rotationPointer  = 0.f;
                m_lastPointerDir = geom::getLeftVector();
            }
        }
        thrusterPos = m_pos.translated (geom::getDownVector() * thrusterOffsetFromCore);
    }

    geom::Vector2D playerInputVec = input::getAxisVec();
    if (playerInputVec.getSquareMagnitude() > geom::precision::quarterPixel) {
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
            thrusterPos      = m_pos.translated (geom::getRightVector() * thrusterOffsetFromCore);
            break;
        case geom::Quadrant::Y_ALIGNED:
            rotationThruster = 90.f;
            thrusterPos      = m_pos.translated (geom::getDownVector() * thrusterOffsetFromCore);
            break;
        case geom::Quadrant::X_OPPOSITE:
            rotationThruster = 180.f;
            thrusterPos      = m_pos.translated (geom::getLeftVector() * thrusterOffsetFromCore);
            break;
        case geom::Quadrant::Y_OPPOSITE:
            rotationThruster = -90.f;
            thrusterPos      = m_pos.translated (geom::getUpVector() * thrusterOffsetFromCore);
            break;
        default:
            break;
        }
    }

    auto& textureManager = TextureManager::getInstance();
    textureManager.draw (spriteInfo.id, corePos);
    // std::print ("Rotation for pointer is {}\n", rotationPointer);

    geom::Point2D pointerPos = m_pos.translated (pointerOffset * pointerOffsetFromCore);
    if (m_currentPointerSprite == PointerState::Shield) {
        pointerPos = m_pos.translated (pointerOffset * shieldOffsetFromCore);
    }
    auto          pointerSpriteInfo = getSpriteInfo (convertToSprite (m_currentPointerSprite));
    geom::Point2D pointerOrigin (pointerSpriteInfo.width / 2.f, pointerSpriteInfo.height / 2.f);
    textureManager.drawRotated (getTextureId (convertToSprite (m_currentPointerSprite)), pointerPos,
                                pointerSpriteInfo.width, pointerSpriteInfo.height, pointerOrigin, rotationPointer);

    auto          thrusterSpriteInfo = graphics::getSpriteInfo (graphics::PlayerSprite::BodyThruster);
    geom::Point2D thrusterOrigin (thrusterSpriteInfo.width / 2.f, thrusterSpriteInfo.height / 2.f);
    textureManager.drawRotated (graphics::getTextureId (graphics::PlayerSprite::BodyThruster), thrusterPos,
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
    m_pos.translate (newPhysState.trsl);

    m_pos.x = std::round (m_pos.x); // snapping character to the pixel grid
    m_pos.y = std::round (m_pos.y); // snapping character to the pixel grid

    m_nextAction = Action::Idle;

    debug::log (std::move (getCollider()));
    debug::log (std::move (getCollider (ColliderType::INTERACTION)), RED);
    // debug::log (updateState, m_pos);
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
        float minX = m_pos.x - shieldRadius;
        float minY = m_pos.y - shieldRadius; // adding hover distance above as well

        float maxX = minX + 2 * shieldRadius;
        float maxY = minY + 2 * shieldRadius + groundHoverDistance;

        geom::Point2D pointerEndPos = m_pos.translated (m_lastPointerDir * (30 + pointerLength));

        minX = std::min (minX, pointerEndPos.x);
        minY = std::min (minY, pointerEndPos.y);

        maxX = std::max (maxX, pointerEndPos.x);
        maxY = std::max (maxY, pointerEndPos.y);

        return physics::Collider (geom::Point2D (minX, minY), geom::Point2D (maxX, maxY));
    }
    case ColliderType::PHYSICAL:
    {
        float minX = m_pos.x - shieldRadius;
        float minY = m_pos.y - shieldRadius; // adding hover distance above as well
        return physics::Collider (geom::Point2D (minX, minY), 2 * shieldRadius + groundHoverDistance, 2 * shieldRadius);
    }
    }
    std::unreachable();
}

Player::~Player()
{
    core::log::warning ("Player has been deleted");
}

void Player::changeMovementMode (MovementMode newMode)
{
    switch (newMode) {
    case MovementMode::None:
    {
        // m_anim.SetProps ("mage_idle", 0, 8, m_height, m_width, 0.08f);
        break;
    }
    case MovementMode::Moving:
    {
        // m_anim.SetProps ("mage_run", 0, 8, m_height, m_width, 0.08f);
        break;
    }
    case MovementMode::AirMoving:
        // m_anim.SetProps ("mage_idle", 0, 8, m_height, m_width, 0.08f);
        break;
    default:
        break;
    }
    m_currentMovementMode = newMode;
}

void MovePlayer (Player& player)
{
    player.addAction (Action::Move);
}

void JumpPlayer (Player& player)
{
    player.addAction (Action::Jump);
}

} // namespace player
