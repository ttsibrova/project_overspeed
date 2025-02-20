#pragma once
#include <Geom/Vector2D.h>
#include <Player/PlayerPhysicalState.h>

#include <optional>

namespace physics::movement {

struct UpdateState
{
    player::MovementMode nextMode = player::MovementMode::NONE;
    geom::Vector2D       velocity;
    geom::Vector2D       trsl;
    float                simTime = 0.f;
};

} // namespace physics::movement
