#include <iostream>
#include <unistd.h>
#include "Game.h"

using namespace std;

int main(int argc, char* args[])
{
    Game game;
    for (;;)
    {
        game.runCPU();
        game.updateWindow();
        usleep(1000);
    }
}
