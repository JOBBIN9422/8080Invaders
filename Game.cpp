#include "Game.h"
using namespace std;

Game::Game() : cpu(0)
{
    //load Space Invaders ROM
    cpu.readIntoMem("invaders.h", 0);
    cpu.readIntoMem("invaders.g", 0x800);
    cpu.readIntoMem("invaders.f", 0x1000);
    cpu.readIntoMem("invaders.e", 0x1800);

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "SDL init error: " << SDL_GetError() << endl;
    }
    else
    {
        mainWin = SDL_CreateWindow("TEST", SDL_WINDOWPOS_UNDEFINED, 
                                   SDL_WINDOWPOS_UNDEFINED, 256, 224, SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(mainWin, -1, SDL_RENDERER_ACCELERATED);
        windowTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, 
                                         SDL_TEXTUREACCESS_STATIC, 256, 224);
        //SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xff, 0xff, 0xff));
        pixelBuffer = new uint[256 * 224];
    }
}

void Game::copyToPixelBuffer()
{
    array<unsigned char, 0x10000> memory = cpu.getMemory();
    int pixelIndex = 0; 

    for (int i = 0x2400; i < 0x3fff; i++)
    {
        unsigned char currByte = memory[i];
        //loop through the 8 bits (pixels) of the current byte 
        for (int j = 0; j < 8; j++)
        {
            if (currByte & 0x01)
            {
                pixelBuffer[pixelIndex] = 0xFFFFFFFF;
            }
            else
            {
                pixelBuffer[pixelIndex] = 0;
            }
            pixelIndex++;
            currByte >>= 1;
        }
    }

}

void Game::updateWindow()
{
    //update pixel buffer from CPU memory map
    copyToPixelBuffer();
    SDL_UpdateTexture(windowTexture, NULL, pixelBuffer, 256 * sizeof(uint));

    //render texture to window
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, windowTexture, NULL, NULL);
    //SDL_RenderCopyEx(renderer, windowTexture, NULL, NULL, -90.0, NULL, SDL_FLIP_NONE);
    SDL_RenderPresent(renderer);
}

void Game::runCPU()
{
    double now = getTimeUsec();
    updateWindow();

    if (lastTimer == 0.0)
    {
        lastTimer = now;
        nextInterrupt = lastTimer + 16000.0;
        whichInterrupt = 1;
    }

    //graphics interrupts
    if (cpu.getInterruptStatus() && (now > nextInterrupt))
    {
        //updateWindow();
        if (whichInterrupt == 1)
        {
            cpu.genInterrupt(1);
            whichInterrupt = 2;
        } 
        else
        {
            cpu.genInterrupt(2);
            whichInterrupt = 1;
        }
        nextInterrupt = now + 8000.0;
    }

    //ensure that CPU performs ~2M cycles/sec (2MHz)
    double sinceLast = now - lastTimer;
    int catchUpCycles = 2 * sinceLast;
    int cycleCount = 0;

    while (catchUpCycles > cycleCount)
    {
        pollKeyboard();
        cycleCount += cpu.emulateCycle();
        //cpu.debugPrint();
    }

    lastTimer = now;
}

double Game::getTimeUsec()
{
    timeval time;
    gettimeofday(&time, NULL);
    return ((double)time.tv_sec * 1E6) + ((double)time.tv_usec);
}

void Game::pollKeyboard()
{
        SDL_PollEvent(&keyEvent);
        switch (keyEvent.type)
        {
            case SDL_KEYDOWN:
                switch (keyEvent.key.keysym.sym)
                {
                    //set port bits here!
                    case SDLK_LEFT: 
                        //cout << "left" << endl; 
                        cpu.setP1Left(true);
                        break;

                    case SDLK_RIGHT: 
                        //cout << "right" << endl; 
                        cpu.setP1Right(true);
                        break;

                    case SDLK_SPACE: 
                        //cout << "fire" << endl; 
                        cpu.setP1Fire(true);
                        break;

                    case SDLK_RETURN: 
                        //cout << "start" << endl; 
                        cpu.setP1Start(true);
                        break;

                    case SDLK_RSHIFT: 
                        //cout << "start" << endl; 
                        cpu.setCoin(true);
                        break;
                }
                break;

            case SDL_KEYUP:
                switch (keyEvent.key.keysym.sym)
                {
                    case SDLK_LEFT: 
                        cpu.setP1Left(false);
                        break;

                    case SDLK_RIGHT: 
                        cpu.setP1Right(false);
                        break;

                    case SDLK_SPACE: 
                        cpu.setP1Fire(false);
                        break;

                    case SDLK_RETURN: 
                        cpu.setP1Start(false);
                        break;

                    case SDLK_RSHIFT: 
                        cpu.setCoin(false);
                        break;
                }
                break;
        }
}
