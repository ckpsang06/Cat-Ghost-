#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "Entity.h"
#include "mainObject.h"

enum GameTextureName {
    BackGroundTexture,
    CatTexture,
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
    Cat cat[TotalCatState];
    Entity heart;

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
};
