#include "Game.h"
#include "CommonFunc.h"

using namespace std;

Game::Game() {
    isRunning = true;
    window = NULL;
    renderer = NULL;
}

bool Game::running() { return isRunning; }

void Game::initSDL(const char *p_title, int p_w, int p_h) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "Failed to initialize SDL: " << SDL_GetError() << "\n";
    }
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) {
        std::cout << "Failed to initialize SDL_image: " << IMG_GetError() << "\n";
    }

    window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

SDL_Texture *Game::loadTexture(const char *p_filePath) {
    SDL_Texture *texture = NULL;
    texture = IMG_LoadTexture(renderer, p_filePath);
    if (texture == NULL) {
        std::cout << "Failed to load texture: " << SDL_GetError() << "\n";
    }
    return texture;
}

void Game::loadMedia() {
    GameTexture[BackGroundTexture] = loadTexture("res/images/background.png");
    GameTexture[CatTexture] = loadTexture("res/images/main.png");
    GameTexture[HeartTexture] = loadTexture("res/images/heart.png");
}

void Game::initEntity() {
    cat[CatIdle].init(GameTexture[CatTexture], 100, 100, 20, 80);
    cat[CatIdle].setHealth(CatHealth);
    cat[CatIdle].setX(SCREEN_WIDTH / 2);
    cat[CatIdle].setY(SCREEN_HEIGHT / 2);

    heart.init(GameTexture[HeartTexture], 50, 50, 2, 0);
}

void Game::clear() {
    SDL_SetRenderDrawColor(renderer, whiteColor.r, whiteColor.g, whiteColor.b, whiteColor.a);
    SDL_RenderClear(renderer);
}

void Game::handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) {
        isRunning = false;
    }
}

void Game::gameUpdate() {
    if (cat[CatIdle].getHealth() > 0) {
        cat[CatIdle].update();
    }
}

void Game::render(SDL_Texture *p_tex) {
    SDL_RenderCopy(renderer, p_tex, NULL, NULL);
}

void Game::render(Entity &p_entity, SDL_Rect src) {
    if (src.w == 0) {
        SDL_QueryTexture(p_entity.getTexture(), NULL, NULL, &src.w, &src.h);
    }
    SDL_Rect dst = {(int)p_entity.getX(), (int)p_entity.getY(), p_entity.getWidth() , p_entity.getHeight()};
    if (p_entity.getFlip() == SDL_FLIP_HORIZONTAL)
        SDL_RenderCopyEx(renderer, p_entity.getTexture(), &src, &dst, 0.0, NULL, SDL_FLIP_HORIZONTAL);
    else
        SDL_RenderCopy(renderer, p_entity.getTexture(), &src, &dst);
}

void Game::gameRender() {
    clear();

    // Vẽ background
    render(GameTexture[BackGroundTexture]);

    // Vẽ Cat đang đứng yên (Idle)
    render(cat[CatIdle], cat[CatIdle].getCurrentFrame());

    // Vẽ thanh máu (Heart)
    int hx = 20, hy = 20;
    for (int i = 1; i <= CatHealth; i++) {
        heart.setX(hx);
        heart.setY(hy);
        if (i <= cat[CatIdle].getHealth()) render(heart, heart.getFrame(1));
        else render(heart, heart.getFrame(0));
        hx += heart.getWidth() + 20;
    }

    SDL_RenderPresent(renderer);
}

void Game::cleanUp() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

void Game::gameLoop() {
    handleEvents();
    gameUpdate();
    gameRender();
}
