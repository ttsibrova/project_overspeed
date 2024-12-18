#pragma once

#include <Physics/Vector2D.h>
#include <Graphics/RenderFlip.h>

#include <raylib/raylib.h>
#include <string>
#include <unordered_map>

struct TilePos;

class TextureManager
{
public:
    static TextureManager& GetInstance();

    bool Load (std::string id, std::string filename);
    //returns hash\id
    [[nodiscard]] size_t Load (std::string filename);
    
    void Drop (std::string id);
    void Clean();

    void Draw (std::string id, const phs::Vector2D& pos);
    void Draw (size_t id, const phs::Vector2D& pos);
    void DrawTile (size_t id, const phs::Vector2D& pos, float width, float height, TilePos tilePos);
    void DrawFrame (std::string id, const phs::Vector2D& pos, float width, float height, int row, int frame, RenderFlip flip = RenderFlip::FLIP_NONE);

public:
    TextureManager (const TextureManager&) = delete;
    TextureManager operator= (const TextureManager&) = delete;

private:
    TextureManager(){}

private:
    std::unordered_map <std::string, Texture2D> m_texturesMap;
    std::unordered_map <size_t, Texture2D> m_hashedTexturesMap;
};
