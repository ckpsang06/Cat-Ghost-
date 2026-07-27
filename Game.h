#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include "Entity.h"
#include "mainObject.h"
#include "Ghost.h"

enum GameStateName {
    GameOpenning,
    GamePlaying,
    GameEnding,
    TotalGameState
};

enum GameTextureName {
    Menu1Texture,
    Menu1_2Texture,
    BackGroundTexture,
    CatTexture,
    GhostTexture,
    HeartTexture,
    TotalGameTexutre
};

enum CatStateName {
    CatIdle,
    TotalCatState
};

class Game {
private:
    bool isRunning;
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Texture* GameTexture[TotalGameTexutre] = {NULL};
    TTF_Font* font = NULL;
    Cat cat[TotalCatState];
    Entity heart;

    Mix_Music* PlayingMusic = NULL;
    Mix_Chunk* ghostDieSound = NULL;

    int GameState;
    //Quản lý ma
    std::vector<Ghost> vecGhost;
    int spawnGhostTime = 0;
    int spawnGhostDelay = 1500; //Tốc độ sinh ma
    int SCORE = 0;
public:
    Game();
    void initSDL(const char *p_title, int p_w, int p_h);
    void loadMedia();
    void initEntity();
    SDL_Texture *loadTexture(const char *p_filePath);

    bool running();
    void clear();
    void handleEvents();
    void gameUpdate();
    void render(SDL_Texture *p_tex);
    void render(Entity &p_entity, SDL_Rect src = {0, 0, 0, 0});
    void gameRender();
    void cleanUp();
    void gameLoop();
<<<<<<< Updated upstream
=======
    bool isHoveringPlay = false;
    void spawnGhost();
>>>>>>> Stashed changes
};
