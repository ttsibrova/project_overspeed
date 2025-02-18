#pragma once

#include <Player/Player.h>
#include <Input/InputHandler.h>
#include <World/Level.h>
#include <Map/RegisteredMaps.h>
#include <optional>

namespace world {

class GameWorld
{
public:
    GameWorld() = delete;
    GameWorld (GameWorld&&) = default;
    GameWorld& operator= (GameWorld&&) = default;
    GameWorld& operator= (const GameWorld&) = default;
    static std::optional <GameWorld> createGameWorld (map::RegisteredMap map);

    void update();
    void draw();

private:
    GameWorld (Level&& lvl);

private:
    player::Player                m_player;
    input::Layer <player::Player> m_playerInputLayer;
    Level                         m_currentLevel;
};

}
