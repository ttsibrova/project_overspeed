#pragma once
#include <Physics/Vector2D.h>

#define UNI_MASS 1.f
#define GRAVITY 9.8f

class RigidBody
{
public:
    RigidBody():
        m_mass (UNI_MASS),
        m_gravity (GRAVITY)
    {}

    //Getters/Setters
    inline void SetMass (float mass) { m_mass = mass; }
    inline float GetMass() const { return m_mass; }

    inline void SetGravity (float gravity) { m_gravity = gravity; }
    inline float GetGravity() const { return m_gravity; }

    inline const phs::Vector2D& GetPosition() { return m_position; }
    inline const phs::Vector2D& GetVelocity() { return m_velocity; }
    inline const phs::Vector2D& GetAcceleration() { return m_acceletation; }


    //Forces
    inline void ApplyForce (phs::Vector2D f) { m_force = f; }
    inline void ApplyForceX (float fX) { m_force.X() = fX; }
    inline void ApplyForceY (float fY) { m_force.Y() = fY; }
    inline void ResetForce() { m_force = phs::Vector2D (); }

    //Friction
    inline void ApplyFriction (phs::Vector2D friction) { m_friction = friction; }
    inline void ResetFriction() {m_friction = phs::Vector2D(); }

    //Update
    void Update (float dt);


private:
    float m_mass;
    float m_gravity;

    phs::Vector2D m_force;
    phs::Vector2D m_friction;

    phs::Vector2D m_position;
    phs::Vector2D m_velocity;
    phs::Vector2D m_acceletation;
};

