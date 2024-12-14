#pragma once

#include <Object/IObject.h>
#include <Physics/Trsf2D.h>
#include <Physics/Vector2D.h>

struct Properties
{
    Properties (std::string texID, float width, float height, phs::Vector2D pos):
        TextureID (std::move (texID)),
        Width (width),
        Height (height),
        Pos (std::move (pos))
    {}

    std::string TextureID;
    float Width;
    float Height;
    phs::Vector2D Pos;
};


class GameObject : public IObject
{
public:
    GameObject (const Properties& props):
        m_pos (props.Pos)
    {}

    virtual ~GameObject() {}

public:
    const phs::Vector2D& Pos() const { return m_pos; }
    phs::Vector2D& Pos() { return m_pos; }

    const phs::Trsf2D& Trsf() const { return m_trsf; }
    phs::Trsf2D& Trsf() { return m_trsf; }

    phs::Vector2D TransformedPos() const { return m_pos.Transformed (m_trsf); }

private:
    phs::Vector2D m_pos;
    phs::Trsf2D m_trsf;

};
