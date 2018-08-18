#include <SDL2/SDL.h>
#include <sys/time.h>
#include "CPU.h"

//Controls the CPU, renders game to window, handles key events
class Game
{
    private:
        CPU cpu;
        
        double lastTimer;
        double nextInterrupt;

        int whichInterrupt;

        SDL_Event keyEvent;
        SDL_Window* mainWin;
        SDL_Renderer* renderer;
        SDL_Texture* windowTexture;
        uint* pixelBuffer;

    public:
        Game();
        void pollKeyboard();
        void copyToPixelBuffer();

        void updateWindow();

        void runCPU();

        double getTimeUsec();
};
