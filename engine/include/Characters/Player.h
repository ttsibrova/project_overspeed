#pragma once

#include <Animation/Animation.h>
#include <Characters/Character.h>
#include <Characters/MovementComponent.h>
#include <Inputs/Command.h>
#include <Physics/RigidBody.h>


class MageMoveCommand: public Command
{
public:
    virtual bool Execute (IObject* obj) override;
};

class MageJumpCommand: public Command
{
public:
    virtual bool Execute (IObject* obj) override;
};


class Player: public Character
{
public:
    Player (Properties props);

    virtual void Draw() override;
    virtual void Update (double dt) override;
    virtual void Clean() override {}

    void Move();
    void Jump();

private:
    Animation         m_anim;
    RenderFlip        m_flip;
    MovementComponent m_mc;

    float m_height;
    float m_width;
};

