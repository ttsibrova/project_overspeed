#pragma once
#include <Characters/PlayerStates.h>
#include <Physics/Vector2D.h>
#include <Physics/Trsf2D.h>

#include <optional>

struct PhysicsUpdateState
{
    player::MovementMode nextMode = player::MovementMode::NONE;
    phs::Vector2D velocity;
    phs::Vector2D trsl;
    float         simTime = 0.f;
};

