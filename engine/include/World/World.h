#pragma once

#include <Characters/Player.h>
#include <Inputs/InputHandler.h>


class GameWorld
{
public:
    void Init();
    void Update();

    void LoadLevel();

private:
    Player m_player;
    InputLayer <Player> m_playerInputLayer;
};
