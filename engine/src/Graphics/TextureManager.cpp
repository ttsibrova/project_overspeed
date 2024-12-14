#include <Graphics/TextureManager.h>

TextureManager& TextureManager::GetInstance()
{
    static TextureManager manager;
    return manager;
}

bool TextureManager::Load (std::string id, std::string filename)
{
    auto texture = LoadTexture (filename.c_str());
    if (!IsTextureReady (texture))
        return false;

    m_texturesMap[id] = texture;
    return true;
}

void TextureManager::Drop (std::string id)
{
    UnloadTexture (m_texturesMap[id]);
    m_texturesMap.erase (id);
}

void TextureManager::Clean()
{
    for (auto [key, val] : m_texturesMap) {
        UnloadTexture (val);
    }
    m_texturesMap.clear();
}

void TextureManager::Draw (std::string id, const phs::Vector2D& pos)
{
    DrawTextureV (m_texturesMap[id], pos, WHITE);
}

void TextureManager::DrawFrame (std::string id, const phs::Vector2D& pos, float width, float height, int row, int frame, RenderFlip flip)
{
    float x0 = frame * width;
    float y0 = row * height;

    if (flip & RenderFlip::FLIP_VERTICAL) {
        height = -height;
    }
    if (flip & RenderFlip::FLIP_HORIZONTAL) {
        width = -width;
    }
    Rectangle rec {x0, y0, width, height};
    DrawTextureRec (m_texturesMap[id], rec, pos, WHITE);
}



