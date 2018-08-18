#include <iostream>
#include <unistd.h>
#include "Game.h"

using namespace std;

int main(int argc, char* args[])
{
    //space invaders starts at 0x0, test prog starts at 0x100
    Game game;

    for (;;)
    {
        game.runCPU();
        usleep(1000);
    }
}
