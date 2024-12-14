#include <Physics/RigidBody.h>

void RigidBody::Update (float dt)
{
    m_acceletation.X() = (m_force.X() + m_friction.X()) / m_mass;
    m_acceletation.Y() = m_gravity + m_force.Y() / m_mass;
    m_velocity += m_acceletation * dt;
    phs::Log (m_velocity, "m_velocity = ");
    //m_acceletation.Log ("m_acceletation = ");
    m_position += m_velocity * dt;
    //m_position.Log ("Distance = ");
}
