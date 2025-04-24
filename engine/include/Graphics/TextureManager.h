#pragma once

#include <Geom/Point2D.h>
#include <Graphics/RenderFlip.h>

#include <optional>
#include <raylib.h>
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

} // namespace graphics

class TextureManager
{
public:
    static TextureManager& getInstance();
    static size_t          getMissingId();

    // returns hash\id
    [[nodiscard]] std::optional<size_t> Load (std::string filename);
    bool                                Load (size_t id, std::string filename);

    void clean();

    void draw (size_t id, const geom::Point2D& pos) const;
    void drawRotated (size_t               id,
                      const geom::Point2D& pos,
                      float                width,
                      float                height,
                      const geom::Point2D& origin,
                      float                rot) const;

    void drawTile (size_t id, const geom::Point2D& pos, float width, float height, map::TiledGridPositon tilePos) const;
    void drawMissing (const geom::Point2D& pos, float width, float height, Color color) const;
    // void drawFrame (std::string id, const geom::Point2D& pos, float width, float height, int row, int frame,
    //                 graphics::RenderFlip flip = graphics::RenderFlip::FlipNone);

    graphics::ImageInfo getImageInfo (size_t id) const;

public:
    TextureManager (const TextureManager&)           = delete;
    TextureManager operator= (const TextureManager&) = delete;

private:
    TextureManager() {}

    std::optional<Texture2D> getTexture (size_t id) const;

private:
    std::unordered_map<size_t, Texture2D> m_hashedTexturesMap;
};
