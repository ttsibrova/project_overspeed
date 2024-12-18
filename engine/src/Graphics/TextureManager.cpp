#include <Graphics/TextureManager.h>
#include <Map/TilesetImageData.h>


TextureManager& TextureManager::GetInstance()
{
    static TextureManager manager;
    return manager;
}

bool TextureManager::Load (std::string id, std::string filename)
{
    if (m_texturesMap.contains (id)) {
        return true;
    }
    auto texture = LoadTexture (filename.c_str());
    if (!IsTextureReady (texture))
        return false;

    m_texturesMap[id] = texture;
    return true;
}

size_t TextureManager::Load (std::string filename)
{
    auto hash = std::hash<std::string>{} (filename);
    if (m_hashedTexturesMap.contains (hash)) {
        return hash;
    }
    auto texture = LoadTexture (filename.c_str());
    if (!IsTextureReady (texture))
        return 0;

    m_hashedTexturesMap[hash] = texture;
    return hash;
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
    for (auto [key, val] : m_hashedTexturesMap) {
        UnloadTexture (val);
    }
    m_texturesMap.clear();
    m_hashedTexturesMap.clear();
}

void TextureManager::Draw (std::string id, const phs::Vector2D& pos)
{
    DrawTextureV (m_texturesMap[id], pos, WHITE);
}

void TextureManager::Draw (size_t id, const phs::Vector2D& pos)
{
    DrawTextureV (m_hashedTexturesMap[id], pos, WHITE);
}

void TextureManager::DrawTile (size_t id, const phs::Vector2D& pos, float width, float height, TilePos tilePos)
{
    float x0 = tilePos.col * width;
    float y0 = tilePos.row * height;

    Rectangle rec {x0, y0, width, height};
    DrawTextureRec (m_hashedTexturesMap[id], rec, pos, WHITE);
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



