#include <iostream>
#include <unistd.h>
#include <thread>
#include "Game.h"

using namespace std;

//helper function, calls window update in a loop
void updateWindow(Game& game)
{
    while(game.isRunning())
    {
        game.updateWindow();
        usleep(1667); //run updates at 60hz
    }
}

void handleIO(Game& game)
{
    while(game.isRunning())
    {
        game.playSounds();
        game.pollKeyboard();
        usleep(1667); //run updates at 60hz
    }
}

int main(int argc, char* args[])
{
    //instantiate game and window update thread
    Game game;
    thread renderThread(updateWindow, ref(game));
    thread IOThread(handleIO, ref(game));

    while (game.isRunning())
    {
        game.runCPU();
        usleep(1000);
    }

    renderThread.join();
    IOThread.join();
    return 0;
}
