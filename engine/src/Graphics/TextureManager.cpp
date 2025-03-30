#include <Graphics/TextureManager.h>
#include <Map/TiledGridPosition.h>
#include <World/Settings.h>

TextureManager& TextureManager::getInstance()
{
    static TextureManager manager;
    return manager;
}

size_t TextureManager::getMissingId()
{
    return 0u;
}

bool TextureManager::Load (std::string id, std::string filename)
{
    if (m_texturesMap.contains (id)) {
        return true;
    }
    auto texture = LoadTexture (filename.c_str());
    if (!IsTextureValid (texture)) {
        return false;
    }

    m_texturesMap[id] = texture;
    return true;
}

std::optional<size_t> TextureManager::Load (std::string filename)
{
    auto hash = std::hash<std::string> {}(filename);
    if (m_hashedTexturesMap.contains (hash)) {
        return hash;
    }
    auto texture = LoadTexture (filename.c_str());
    if (!IsTextureValid (texture)) {
        return std::nullopt;
    }

    m_hashedTexturesMap[hash] = texture;
    return hash;
}

bool TextureManager::Load (size_t id, std::string filename)
{
    if (m_hashedTexturesMap.contains (id)) {
        return true;
    }
    auto texture = LoadTexture (filename.c_str());
    if (!IsTextureValid (texture)) {
        return false;
    }

    m_hashedTexturesMap[id] = texture;
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
    for (auto [key, val] : m_hashedTexturesMap) {
        UnloadTexture (val);
    }
    m_texturesMap.clear();
    m_hashedTexturesMap.clear();
}

void TextureManager::draw (std::string id, const geom::Point2D& pos)
{
    DrawTextureV (m_texturesMap[id], pos, WHITE);
}

void TextureManager::draw (size_t id, const geom::Point2D& pos)
{
    if (id == getMissingId()) {
        return drawMissing(pos, world::settings::tileSize.width, world::settings::tileSize.height, RED);
    }
    DrawTextureV (m_hashedTexturesMap[id], pos, WHITE);
}

void TextureManager::DrawRotated (size_t id, const geom::Point2D& pos, float width, float height,
                                  const geom::Point2D& origin, float rot)
{
    if (id == getMissingId()) {
        return drawMissing (pos, width, height, RED);
    }
    Rectangle source { .x = 0.f, .y = 0.f, .width = width, .height = height };
    Rectangle dest { .x = pos.X(), .y = pos.Y(), .width = width, .height = height };
    DrawTexturePro (m_hashedTexturesMap[id], source, dest, origin, rot, WHITE);
}

void TextureManager::DrawTile (size_t id, const geom::Point2D& pos, float width, float height, map::TiledGridPositon tilePos)
{
    if (id == getMissingId()) {
        return drawMissing (pos, width, height, RED);
    }
    float x0 = tilePos.x * width;
    float y0 = tilePos.y * height;

    Rectangle rec { x0, y0, width, height };
    DrawTextureRec (m_hashedTexturesMap[id], rec, pos, WHITE);
}

void TextureManager::drawMissing (const geom::Point2D& pos, float width, float height, Color color)
{
    Rectangle rectangle { pos.X(), pos.Y(), width, height };
    DrawRectangleRec (rectangle, color);
    DrawRectangleLinesEx (rectangle, 5.f, BLACK);
    DrawRectangleLinesEx (rectangle, 1.f, color);
    DrawLineEx (pos, Vector2 { .x = pos.X() + width, .y = pos.Y() + height }, 5.f, BLACK);
}

void TextureManager::DrawFrame (std::string id, const geom::Point2D& pos, float width, float height, int row, int frame,
                                graphics::RenderFlip flip)
{
    float x0 = frame * width;
    float y0 = row * height;

    if (flip & graphics::RenderFlip::FLIP_VERTICAL) {
        height = -height;
    }
    if (flip & graphics::RenderFlip::FLIP_HORIZONTAL) {
        width = -width;
    }

    Rectangle rec { x0, y0, width, height };
    DrawTextureRec (m_texturesMap[id], rec, pos, WHITE);
}

graphics::ImageInfo TextureManager::getImageInfo (size_t id)
{
    assert(m_hashedTexturesMap.contains(id));
    const auto& texture = m_hashedTexturesMap[id];
    return {
        .width  = static_cast<float> (texture.width),
        .height = static_cast<float> (texture.height),
    };
}
