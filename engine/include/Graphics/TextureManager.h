#pragma once

#include <Geom/Point2D.h>
#include <Graphics/RenderFlip.h>

#include <raylib/raylib.h>
#include <string>
#include <unordered_map>

namespace map {
struct TilePos;
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
    static TextureManager& GetInstance();

    bool Load (std::string id, std::string filename);
    // returns hash\id
    [[nodiscard]] size_t Load (std::string filename); // make it return optional?
    bool                 Load (size_t id, std::string filename);

    void Drop (std::string id);
    void Clean();

    void draw (std::string id, const geom::Point2D& pos);
    void draw (size_t id, const geom::Point2D& pos);
    void DrawRotated (size_t id, const geom::Point2D& pos, float width, float height, const geom::Point2D& origin, float rot);
    void DrawTile (size_t id, const geom::Point2D& pos, float width, float height, map::TilePos tilePos);
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
