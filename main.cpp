#include <SDL.h>
#include "Game.h"
#include "CommonFunc.h"
#include "Entity.h"

Game *game = new Game();

int main(int argc, char* args[]) {
    game->initSDL("Minerva - @tann", SCREEN_WIDTH, SCREEN_HEIGHT);
    game->loadMedia();
    game->initEntity();

    int frameStart, frameTime;

    while (game->running()) {
        frameStart = SDL_GetTicks();
        
        game->gameLoop();
        
        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    game->cleanUp();
    delete game;

    return 0;
}
