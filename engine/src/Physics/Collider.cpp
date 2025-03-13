#include <Physics/Collider.h>

bool physics::Collider::collides (const Collider& other) const
{
    return ((m_min.X() < other.m_max.X() && m_max.X() > other.m_min.X()) &&
            (m_min.Y() < other.m_max.Y() && m_max.Y() > other.m_min.Y()));
}
