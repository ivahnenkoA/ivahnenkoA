// Cluedo board game project
// Anna Ivahnenko  25/05/21

#include <iostream> 
#include <time.h>     
#include <vector>
#include <random>
#include "game_component.h"
#include "game_process.h"
#include "game_component.cpp"
#include "game_process.cpp"

int main()
{
    srand(time(NULL));

    global game;
    std::vector<std::unique_ptr<comp::player>> players = game.start_game();
    turn playing;

    bool quit{false};
    while(!quit)
    {
        for(int i{0}; i < players.size(); i++)
        {
            quit = playing.player_turn(*players[i], players, game);
        }
    }

    return 0;
}