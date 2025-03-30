#pragma once

#include <Geom/Point2D.h>
#include <Graphics/RenderFlip.h>

#include <raylib.h>
#include <optional>
#include <string>
#include <unordered_map>

namespace map {
struct TiledGridPositon;
}

namespace graphics {

struct ImageInfo
{
    float width;
    float height;
};

}

class TextureManager
{
public:
    static TextureManager& getInstance();
    static size_t          getMissingId();

    bool Load (std::string id, std::string filename);
    // returns hash\id
    [[nodiscard]] std::optional<size_t> Load (std::string filename); // make it return optional?
    bool                                Load (size_t id, std::string filename);

    void Drop (std::string id);
    void Clean();

    void draw (std::string id, const geom::Point2D& pos);
    void draw (size_t id, const geom::Point2D& pos);
    void DrawRotated (size_t id, const geom::Point2D& pos, float width, float height, const geom::Point2D& origin, float rot);
    void DrawTile (size_t id, const geom::Point2D& pos, float width, float height, map::TiledGridPositon tilePos);
    void drawMissing (const geom::Point2D& pos, float width, float height, Color color);
    void DrawFrame (std::string id, const geom::Point2D& pos, float width, float height, int row, int frame,
                    graphics::RenderFlip flip = graphics::RenderFlip::FLIP_NONE);

    graphics::ImageInfo getImageInfo(size_t id);

public:
    TextureManager (const TextureManager&)           = delete;
    TextureManager operator= (const TextureManager&) = delete;

private:
    TextureManager() {}

private:
    std::unordered_map<std::string, Texture2D> m_texturesMap;
    std::unordered_map<size_t, Texture2D>      m_hashedTexturesMap;
};
