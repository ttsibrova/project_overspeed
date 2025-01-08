#pragma once
#include <Characters/PlayerMovementMode.h>
#include <Physics/Vector2D.h>
#include <Physics/Trsf2D.h>

struct PhysicsUpdateState
{
    MovementMode nextMode = MovementMode::NONE;
    phs::Vector2D velocity;
    phs::Trsf2D   trsf;
    float         simTime = 0.f;
};

