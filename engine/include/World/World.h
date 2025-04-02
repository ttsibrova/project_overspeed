#pragma once

#include <Map/RegisteredMaps.h>
#include <Player/Player.h>
#include <World/Level.h>

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

private:
    player::Player               m_player;
    Level                        m_currentLevel;
};

struct GameWorldBuilder
{
    map::RegisteredMap map;

    operator GameWorld() const { return world::GameWorld::createGameWorld(map); }
};

} // namespace world
