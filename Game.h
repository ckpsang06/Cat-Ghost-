#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <vector>
#include "Entity.h"
#include "mainObject.h"
#include "Ghost.h"


enum GameStateName {
    GameOpenning,
    GamePlaying,
    GameEnding,
    GameWinning,
    GameLosing,
    GameIntro,
    GameDie,
    GameVictory,

    TotalGameState
};

enum GameTextureName {
    Menu1Texture,
    Menu1_2Texture,
    BackGroundTexture,
    CatTexture,

    GhostTexture,
    HeartTexture,
    Menu2Texture,
    Menu3Texture,
    HomeTexture,
    AgainTexture,
    CatHurtTexture,
    CatAttackTexture,
    CatRunningTexture,
    CatDieTexture,
    CatVictoryTexture,
    CatLightningTexture,
    LightningStrikeTexture,
    TotalGameTexutre
};

enum CatStateName {
    CatIdle,
    CatHurt,
    CatAttack,
    CatRunning,
    CatDie,
    CatVictory,

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
    Entity home;
    Entity again;

    Mix_Music* PlayingMusic = NULL;
    Mix_Chunk* ghostDieSound = NULL;
    Mix_Chunk* thunderSound;

    int GameState;
    //Quản lý ma
    std::vector<Ghost> vecGhost;
    int spawnGhostTime = 0;
    int spawnGhostDelay;// Thời gian sinh ma
    int spawnGhostDelayAfterCatRunning = 1000; // các lần sinh ma sau
    int spawnGhostDelayBeforeCatRunning = 2000; // lần sinh ma đầu tiên
    int SCORE = 0;

    bool isCatAttacking = false;
    bool isCatRunning = false;
    Uint32 attackStartTime = 0;
    Uint32 gameStartTime;
    Uint32 CatDieStartTime;
    Uint32 CatVictoryStartTime;
    const Uint32 GAME_TIME = 50000; // thời gian để chiến thắng
    void resetGame();//Reset các     trạng thái

    //các biến quản lý chiêu sấm sét
    Cat catLightning;
    Cat lightningStrike;
    std::vector<SDL_Rect> lightningTargets;
    bool isUltimateActive;
    Uint32 ultimateStartTime;
    Uint32 lastUltimateTime;

    bool isChargingUltimate; // kiểm tra giữ phím space
    Uint32 chargeStartTime;  // Thời điểm bắt đầu giữ phím

public:
    Game();
    void renderText(const std::string &text, int x, int y, SDL_Color color);
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


    bool isHoveringPlay = false;
    void spawnGhost();
    bool isCatHurt = false;
    Uint32 catHurtStartTime = 0;
};
