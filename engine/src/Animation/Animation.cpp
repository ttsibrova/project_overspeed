#include <Animation/Animation.h>
#include <Graphics/TextureManager.h>
#include <Tools/Assert.h>

void Animation::update()
{
    m_frame = static_cast<int> (GetTime() / m_animSpeed) % m_frameCount;
}

void Animation::draw (const geom::Point2D&, graphics::RenderFlip)
{
    m_assert (false, "Function is not implemented");
    //TextureManager::getInstance().drawFrame (m_textureID, pos, m_width, m_height, m_row, m_frame, flip);
}

void Animation::SetProps (std::string textureID, int row, int frame, float height, float width, float animSpeed)
{
    m_textureID  = textureID;
    m_row        = row;
    m_frameCount = frame;
    m_height     = height;
    m_width      = width;
    m_animSpeed  = animSpeed;
}
