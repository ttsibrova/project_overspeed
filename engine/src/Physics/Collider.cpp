#include <Physics/Collider.h>

bool physics::Collider::collides (const Collider& other) const
{
    return ((m_min.x < other.m_max.x && m_max.x > other.m_min.x) &&
            (m_min.y < other.m_max.y && m_max.y > other.m_min.y));
}
