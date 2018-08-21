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
        const int SCREEN_WIDTH = 256;
        const int SCREEN_HEIGHT = 224;

        SDL_Event keyEvent;
        SDL_Window* mainWin;
        SDL_Renderer* renderer;
        SDL_Texture* windowTexture;
        SDL_Rect flipRect;
        uint* pixelBuffer;
        uint* rotatedBuffer;

    public:
        Game();
        void pollKeyboard();
        void copyToPixelBuffer();

        void updateWindow();
        void clearPixelBuffer();
        void rotatePixelBuffer();

        void runCPU();
        double getTimeUsec();
};
