#include <iostream>
#include <unistd.h>
#include "Game.h"

using namespace std;

int main(int argc, char* args[])
{
    Game game;
    while (game.isRunning())
    {
        game.runCPU();
        game.updateWindow();
        usleep(1000);
    }

    return 0;
}
