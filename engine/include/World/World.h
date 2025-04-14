#pragma once

#include <Input/InputLayer.h>
#include <Map/RegisteredMaps.h>
#include <Player/Player.h>
#include <World/Level.h>

#include <raylib.h>

#include <optional>

namespace world {

class GameWorld
{
public:
    GameWorld()             = delete;
    GameWorld (GameWorld&&) = delete;

    GameWorld& operator= (GameWorld&&)      = delete;
    GameWorld& operator= (const GameWorld&) = delete;

    static GameWorld createGameWorld (map::RegisteredMap map);

    void update();
    void draw();

    ~GameWorld();

private:
    GameWorld (Level&& lvl);

    enum class WorldAction : uint8_t
    {
        None,
        PlayerInteraction
    };
    void setAction (WorldAction action);

private:
    WorldAction    m_action;
    player::Player m_player;
    Level          m_currentLevel;
    input::Layer   m_inputLayer;
    Camera2D       m_camera = { 0 };
};

struct GameWorldBuilder
{
    map::RegisteredMap map;

    operator GameWorld() const { return world::GameWorld::createGameWorld (map); }
};

} // namespace world
