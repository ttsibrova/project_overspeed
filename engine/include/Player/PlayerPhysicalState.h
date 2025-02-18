#pragma once
#include <cstdint>
#include <Geom/Vector2D.h>

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
    geom::Vector2D m_velocity;
};

}

