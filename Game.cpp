#include "Game.h"
using namespace std;

Game::Game() : cpu(0)
{
    //load Space Invaders ROM
    cpu.readIntoMem("roms/invaders.h", 0);
    cpu.readIntoMem("roms/invaders.g", 0x800);
    cpu.readIntoMem("roms/invaders.f", 0x1000);
    cpu.readIntoMem("roms/invaders.e", 0x1800);
    //cpu.copyROM();

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "SDL init error: " << SDL_GetError() << endl;
    }
    else
    {
        //get host monitor size (for window scaling)
        SDL_DisplayMode displayMode;
        SDL_GetCurrentDisplayMode(0, &displayMode);
        int displayHeight = displayMode.h;
        int scaleFactor = displayHeight / SCREEN_HEIGHT;

        mainWin = SDL_CreateWindow("TEST", SDL_WINDOWPOS_UNDEFINED, 
                                   SDL_WINDOWPOS_UNDEFINED, 
                                   scaleFactor * SCREEN_HEIGHT, scaleFactor * SCREEN_WIDTH, 
                                   SDL_WINDOW_SHOWN);

        renderer = SDL_CreateRenderer(mainWin, -1, SDL_RENDERER_ACCELERATED);
        //maintain render aspect ratio in full-screen
        SDL_RenderSetLogicalSize(renderer, SCREEN_HEIGHT, SCREEN_WIDTH);
        windowTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, 
                                         SDL_TEXTUREACCESS_STATIC, SCREEN_HEIGHT, SCREEN_WIDTH);

        pixelBuffer = new uint[SCREEN_WIDTH * SCREEN_HEIGHT];
        rotatedBuffer = new uint[SCREEN_WIDTH * SCREEN_HEIGHT];
    }
}

void Game::rotatePixelBuffer()
{
    for (int i = 0; i < SCREEN_WIDTH; i++)
    {
        for (int j = 0; j < SCREEN_HEIGHT; j++)
        {
            rotatedBuffer[j + i * SCREEN_HEIGHT] = pixelBuffer[i + j * SCREEN_WIDTH];
        }
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
    //rotate the 1D pixel array to fit portrait window
    rotatePixelBuffer();
}

void Game::clearPixelBuffer()
{
    for (int i = 0; i < SCREEN_HEIGHT*SCREEN_WIDTH; i++)
    {
        pixelBuffer[i] = 0;
    }
}
void Game::updateWindow()
{
    //update pixel buffer from CPU memory map
    copyToPixelBuffer();
    SDL_UpdateTexture(windowTexture, NULL, rotatedBuffer, SCREEN_HEIGHT * sizeof(uint));

    //render texture to window (flip vertically to make screen upright portrait)
    SDL_RenderClear(renderer);
    SDL_RenderCopyEx(renderer, windowTexture, NULL, NULL, NULL, NULL, SDL_FLIP_VERTICAL);
    SDL_RenderPresent(renderer);
}

void Game::runCPU()
{
    double now = getTimeUsec();

    if (lastTimer == 0.0)
    {
        lastTimer = now;
        nextInterrupt = lastTimer + 16000.0;
        whichInterrupt = 1;
    }

    //graphics interrupts
    if (cpu.getInterruptStatus() && (now > nextInterrupt))
    {
        if (whichInterrupt == 1)
        {
            cpu.genInterrupt(1);
            whichInterrupt = 2;
            //updateWindow();
        } 
        else
        {
            cpu.genInterrupt(2);
            whichInterrupt = 1;
            //updateWindow();
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

                    case SDLK_TAB: 
                        cpu.setDebug(true);
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

                    case SDLK_TAB: 
                        cpu.setDebug(false);
                        break;
                }
                break;
        }
}
