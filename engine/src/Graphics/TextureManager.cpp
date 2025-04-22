#include <Core/Logger.h>
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

void TextureManager::clean()
{
    for (auto [key, val] : m_hashedTexturesMap) {
        UnloadTexture (val);
    }
    m_hashedTexturesMap.clear();
}

void TextureManager::draw (size_t id, const geom::Point2D& pos) const
{
    auto texture = getTexture(id);
    if (!texture.has_value()) {
        return drawMissing(pos, world::settings::tileSize.width, world::settings::tileSize.height, RED);
    }
    DrawTextureV (*texture, pos, WHITE);
}

void TextureManager::drawRotated (size_t id, const geom::Point2D& pos, float width, float height,
                                  const geom::Point2D& origin, float rot) const
{
    auto texture = getTexture (id);
    if (!texture.has_value()) {
        return drawMissing (pos, width, height, RED);
    }
    Rectangle source { .x = 0.f, .y = 0.f, .width = width, .height = height };
    Rectangle dest { .x = pos.X(), .y = pos.Y(), .width = width, .height = height };
    DrawTexturePro (*texture, source, dest, origin, rot, WHITE);
}

void TextureManager::drawTile (size_t id, const geom::Point2D& pos, float width, float height, map::TiledGridPositon tilePos) const
{
    auto texture = getTexture (id);
    if (!texture.has_value()) {
        return drawMissing (pos, width, height, RED);
    }
    float x0 = tilePos.x * width;
    float y0 = tilePos.y * height;

    Rectangle rec { x0, y0, width, height };
    DrawTextureRec (*texture, rec, pos, WHITE);
}

void TextureManager::drawMissing (const geom::Point2D& pos, float width, float height, Color color) const
{
    Rectangle rectangle { pos.X(), pos.Y(), width, height };
    DrawRectangleRec (rectangle, color);
    DrawRectangleLinesEx (rectangle, 5.f, BLACK);
    DrawRectangleLinesEx (rectangle, 1.f, color);
    DrawLineEx (pos, Vector2 { .x = pos.X() + width, .y = pos.Y() + height }, 5.f, BLACK);
}

//void TextureManager::drawFrame (std::string id, const geom::Point2D& pos, float width, float height, int row, int frame,
//                                graphics::RenderFlip flip)
//{
//    float x0 = frame * width;
//    float y0 = row * height;
//
//    if (flip & graphics::RenderFlip::FLIP_VERTICAL) {
//        height = -height;
//    }
//    if (flip & graphics::RenderFlip::FLIP_HORIZONTAL) {
//        width = -width;
//    }
//
//    Rectangle rec { x0, y0, width, height };
//    DrawTextureRec (m_texturesMap[id], rec, pos, WHITE);
//}

graphics::ImageInfo TextureManager::getImageInfo (size_t id) const
{
    auto texture = getTexture (id);
    if (texture.has_value()) {
        return {
            .width  = static_cast<float> (texture->width),
            .height = static_cast<float> (texture->height),
        };
    }
    core::log::warning (std::format ("Fallback sizes for texture ID: {} was generated", id));
    return {
        .width  = world::settings::tileSize.width,
        .height = world::settings::tileSize.height,
    };
}

std::optional<Texture2D> TextureManager::getTexture (size_t id) const
{
    if (id == getMissingId()) {
        return std::nullopt;
    }
    const auto it = m_hashedTexturesMap.find(id);
    if (it != m_hashedTexturesMap.end()) {
        return it->second;
    }
    core::log::warning (std::format ("Missing expected texture ID: {}", id));
    return std::nullopt;
}
