#include <iostream>
#include <unistd.h>
#include "Game.h"

using namespace std;

Uint32 updateGameWindow(Uint32 interval, void* param)
{
    Game* game = (Game*)param;
    game->updateWindow();
}

int main(int argc, char* args[])
{
    //space invaders starts at 0x0, test prog starts at 0x100
    Game game;
    //SDL_AddTimer(16, updateGameWindow, &game);

    for (;;)
    {
        game.runCPU();
        game.updateWindow();
        usleep(1000);
    }
}
