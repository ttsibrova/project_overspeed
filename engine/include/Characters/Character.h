#pragma once

#include <Object/GameObject.h>

class Character: public GameObject
{
public:
    Character (Properties props) :
        GameObject (props),
        m_forwardDir (phs::Vector2D (1., 0.))
    {}

public:
    const std::string& Name() const { return m_name; }
    std::string& Name() { return m_name; }

    const phs::Vector2D& Acceleration() const { return m_acceleration; }
    phs::Vector2D& Acceleration() { return m_acceleration; }

    const phs::Vector2D& Velocity() const { return m_velocity; }
    phs::Vector2D& Velocity() { return m_velocity; }

    const phs::Vector2D& ForwardDir() const { return m_forwardDir; }
    phs::Vector2D& ForwardDir() { return m_forwardDir; }

private:
    std::string m_name;
    phs::Vector2D m_acceleration;
    phs::Vector2D m_velocity;
    phs::Vector2D m_forwardDir;
};

