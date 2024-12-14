#pragma once

#include <Physics/Vector2D.h>
#include <Graphics/RenderFlip.h>

#include <raylib/raylib.h>
#include <string>
#include <unordered_map>


class TextureManager
{
public:
    static TextureManager& GetInstance();

    bool Load (std::string id, std::string filename);
    void Drop (std::string id);
    void Clean();

    void Draw (std::string id, const phs::Vector2D& pos);
    void DrawFrame (std::string id, const phs::Vector2D& pos, float width, float height, int row, int frame, RenderFlip flip = RenderFlip::FLIP_NONE);

public:
    TextureManager (const TextureManager&) = delete;
    TextureManager operator= (const TextureManager&) = delete;

private:
    TextureManager(){}

private:
    std::unordered_map <std::string, Texture2D> m_texturesMap;
};
