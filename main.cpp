#include <iostream>
#include <SDL2/SDL.h>
#include "CPU.h"
using namespace std;

int main(int argc, char* args[])
{
    //space invaders starts at 0x0, test prog starts at 0x100
    CPU cpu(0x100);

    SDL_Event event;
    //SDL_Window* mainWin = NULL;
    //SDL_Surface* screenSurface = NULL;

    //if (SDL_Init(SDL_INIT_VIDEO) < 0)
    //{
    //    cout << "SDL init error: " << SDL_GetError() << endl;
    //}
    //else
    //{
    //    mainWin = SDL_CreateWindow("TEST", SDL_WINDOWPOS_UNDEFINED, 
    //    SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    //    screenSurface = SDL_GetWindowSurface(mainWin);
    //    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xff, 0xff, 0xff));
    //    SDL_UpdateWindowSurface(mainWin);
    //}

    //load Space Invaders ROM
    //cpu.readIntoMem("invaders.h", 0);
    //cpu.readIntoMem("invaders.g", 0x800);
    //cpu.readIntoMem("invaders.f", 0x1000);
    //cpu.readIntoMem("invaders.e", 0x1800);

    cpu.readIntoMem("cpudiag.bin", 0x100);
    cpu.setByte(368, 0x7);
    cpu.setByte(0x59c, 0xc3);
    cpu.setByte(0x59d, 0xc2);
    cpu.setByte(0x59e, 0x05);

    for (;;)
    {

        SDL_PollEvent(&event);
        switch (event.type)
        {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_LEFT: cout << "left" << endl; break;
                }
                break;
        }
        cpu.emulateCycle();
        cpu.debugPrint();
    }
}
