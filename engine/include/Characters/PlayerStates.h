#pragma once
#include <cstdint>
#include <Physics/Vector2D.h>

namespace player {

enum class Action: uint8_t
{
    IDLE,
    MOVE,
    SLIDE,
    JUMP,
};

enum class MovementMode: uint8_t
{
    NONE,
    RUNNING,
    JUMPING,
    AIR_MOVEMENT
};

struct PhysicalState {
    Action nextAction;
    MovementMode currentMode;
    float currentSimTime;
    phs::Vector2D m_velocity;
};


enum class ColliderType: uint8_t
{
    UNDEFINED,
    INTERACTION,
    PHYSICAL
};


enum class CoreState: uint8_t
{
    FULL,
    EMPTY
};

enum class PointerState: uint8_t
{
    SHARP,
    SHIELD
};

struct BodyState {
    CoreState core;
    PointerState pointer;
};

}

