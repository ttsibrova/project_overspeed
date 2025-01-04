#pragma once

#include <Characters/Player.h>
#include <Inputs/InputHandler.h>
#include <World/Level.h>
#include <Map/RegisteredMaps.h>
#include <optional>


class GameWorld
{
public:
    GameWorld() = delete;
    GameWorld (GameWorld&&) = default;
    GameWorld& operator= (GameWorld&&) = default;
    GameWorld& operator= (const GameWorld&) = default;
    static std::optional <GameWorld> CreateGameWorld (Maps map);

    void Update();
    void Draw();

private:
    GameWorld (Level&& lvl);

private:
    Player m_player;
    InputLayer <Player> m_playerInputLayer;
    Level m_currentLevel;
};
