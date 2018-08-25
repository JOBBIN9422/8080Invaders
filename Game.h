#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
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

        //track sound state 
        unsigned char lastSoundPort3;
        unsigned char lastSoundPort5;

        bool loopUFOSound;
        bool exit;

        std::array<Mix_Chunk*, 9> sounds;

        SDL_Event keyEvent;
        SDL_Window* mainWin;
        SDL_Renderer* renderer;
        SDL_Texture* windowTexture;
        SDL_Rect flipRect;
        uint* pixelBuffer;

    public:
        Game();
        void pollKeyboard();
        void copyToPixelBuffer();

        void initVideo();
        void updateWindow();
        void clearPixelBuffer();
        void rotatePixelBuffer();

        void initSound();
        void playSounds();
        void runCPU();
        double getTimeUsec();

        bool isRunning();
};
