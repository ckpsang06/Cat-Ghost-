
 #include "Game.h"
#include "CommonFunc.h"

using namespace std;

Game::Game() {
    isRunning = true;
    window = NULL;
    renderer = NULL;
    GameState = GameOpenning;
}

bool Game::running() { return isRunning; }
void Game::initSDL(const char *p_title, int p_w, int p_h) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << "Failed to initialize SDL: " << SDL_GetError() << "\n";

    }
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) {
        cout << "Failed to initialize SDL_image: " << IMG_GetError() << "\n";
    }
    window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    isHoveringPlay = false;
}

SDL_Texture *Game::loadTexture(const char *p_filePath) {
    SDL_Texture *texture = NULL;
    texture = IMG_LoadTexture(renderer, p_filePath);
    if (texture == NULL) {
        cout << "Failed to load texture: " << SDL_GetError() << "\n";
    }
    return texture;
}

void Game::loadMedia() {
    GameTexture[BackGroundTexture] = loadTexture("res/images/background.png");
    GameTexture[Menu1Texture] = loadTexture("res/images/menu1.png");
    GameTexture[Menu1_2Texture] = loadTexture("res/images/menu1_2.png");
    GameTexture[CatTexture] = loadTexture("res/images/main.png");
    GameTexture[GhostTexture] = loadTexture("res/images/ghost.png");
    GameTexture[HeartTexture] = loadTexture("res/images/heart.png");
    GameTexture[Menu2Texture] = loadTexture("res/images/menu2.png");
    GameTexture[HomeTexture] = loadTexture("res/images/home.png");
    GameTexture[AgainTexture] = loadTexture("res/images/again.png");
    GameTexture[Menu3Texture] = loadTexture("res/images/menu3.png");
    GameTexture[CatHurtTexture] = loadTexture("res/images/hurt.png");
    GameTexture[CatAttackTexture] = loadTexture("res/images/attack.png");
    //fonts
    font = TTF_OpenFont("res/fonts/arial.ttf", 50);
    TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
    if (!font) {
        cout << "Failed to load font: " << TTF_GetError();
    }

    //musics
    PlayingMusic = Mix_LoadMUS("res/sounds/backgroundsound.mp3");
    ghostDieSound = Mix_LoadWAV("res/sounds/die.ogg");
}

void Game::initEntity() {
    cat[CatIdle].init(GameTexture[CatTexture], 100, 100, 20, 80); // Đã update delay animation
    cat[CatIdle].setHealth(CatHealth);
    cat[CatIdle].setX(SCREEN_WIDTH / 2);
    cat[CatIdle].setY(SCREEN_HEIGHT / 2);

    cat[CatHurt].init(GameTexture[CatHurtTexture], 100, 100, 4, 100);
    cat[CatHurt].setX(SCREEN_WIDTH / 2);
    cat[CatHurt].setY(SCREEN_HEIGHT / 2);

    cat[CatAttack].init(GameTexture[CatAttackTexture], 200, 200, 6, 80);
    cat[CatAttack].setX(cat[CatIdle].getX() - 50);
    cat[CatAttack].setY(cat[CatIdle].getY() - 50);

    heart.init(GameTexture[HeartTexture], 50, 50, 2, 0);

    home.init(GameTexture[HomeTexture], 140, 260, 2, 0);
    home.setX(702);
    home.setY(461);

    again.init(GameTexture[AgainTexture], 140, 140, 2, 0);
    again.setX(930);
    again.setY(518);
}

void Game::spawnGhost() {
    Ghost newGhost;
    // Khởi tạo kích thước và frame cho ma
    newGhost.init(GameTexture[GhostTexture], 150, 150, 8, 100);

    // Random vị trí xuất hiện ở 2 bên mép màn hình
    int side = RandNum(0, 1);
    newGhost.setX(side ? SCREEN_WIDTH : 0);
    newGhost.setY(RandNum(0, SCREEN_HEIGHT));
    if (side == 0) newGhost.setFlip(SDL_FLIP_HORIZONTAL);

    vecGhost.push_back(newGhost);
}

void Game::clear() {
    SDL_SetRenderDrawColor(renderer, whiteColor.r, whiteColor.g, whiteColor.b, whiteColor.a);
    SDL_RenderClear(renderer);
}

void Game::handleEvents() {
    SDL_Event event;
    int mouseX, mouseY;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }

        // --- MÀN HÌNH MENU ---
        if (GameState == GameOpenning) {
            SDL_GetMouseState(&mouseX, &mouseY);
            SDL_Rect playButton = { 600, 80, 270, 356 };

            bool isInside = (mouseX >= playButton.x && mouseX <= playButton.x + playButton.w &&
                             mouseY >= playButton.y && mouseY <= playButton.y + playButton.h);

            if (event.type == SDL_MOUSEMOTION) {
                isHoveringPlay = isInside;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && isInside) {
                GameState = GamePlaying;
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                GameState = GamePlaying;
            }
        }
        // --- MÀN HÌNH CHƠI ---
        else if (GameState == GamePlaying) {
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                GameState = GameWinning;
            }

            // Xử lý sự kiện Mèo tấn công khi click chuột
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                if (!isCatHurt && !isCatAttacking) { // Đang bị thương thì ko đánh được
                    isCatAttacking = true;
                    attackStartTime = SDL_GetTicks();
                    cat[CatAttack].setCurrentFrame(0);
                }
            }
        }
        // --- MÀN HÌNH KẾT THÚC (THẮNG / THUA) ---
        else if (GameState == GameWinning || GameState == GameLosing) {
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int clickX = event.button.x;
                int clickY = event.button.y;

                if (clickX >= home.getX() && clickX <= home.getX() + home.getWidth() &&
                    clickY >= home.getY() && clickY <= home.getY() + home.getHeight()) {
                    GameState = GameOpenning;
                }

                if (clickX >= again.getX() && clickX <= again.getX() + again.getWidth() &&
                    clickY >= again.getY() && clickY <= again.getY() + again.getHeight()) {
                    GameState = GamePlaying;
                    cat[CatIdle].setHealth(CatHealth);
                    cat[CatIdle].setX(SCREEN_WIDTH / 2);
                    cat[CatIdle].setY(SCREEN_HEIGHT / 2);
                    vecGhost.clear();
                }
            }
        }
    }

    // Hiệu ứng Hover cho nút Home/Again ở màn hình Thắng/Thua
    SDL_GetMouseState(&mouseX, &mouseY);
    if (GameState == GameWinning || GameState == GameLosing) {
        if (mouseX >= home.getX() && mouseX <= home.getX() + home.getWidth() &&
            mouseY >= home.getY() && mouseY <= home.getY() + home.getHeight()) {
            home.setCurrentFrame(1);
        } else {
            home.setCurrentFrame(0);
        }

        if (mouseX >= again.getX() && mouseX <= again.getX() + again.getWidth() &&
            mouseY >= again.getY() && mouseY <= again.getY() + again.getHeight()) {
            again.setCurrentFrame(1);
        } else {
            again.setCurrentFrame(0);
        }
    }
}

void Game::gameUpdate() {
    if (GameState != GamePlaying) return;

    if (SDL_GetTicks() - spawnGhostTime > spawnGhostDelay) {
        spawnGhost();
        spawnGhostTime = SDL_GetTicks();
    }

    if (cat[CatIdle].getHealth() <= 0) {
        Mix_HaltMusic();
        GameState = GameLosing;
        return;
    }

    for (int i = 0; i < (int)vecGhost.size(); i++) {
        auto &curGhost = vecGhost[i];
        if (curGhost.isDead()) continue;

        if (curGhost.isAttacking()) {
            curGhost.update();
            continue;
        }

        int ghostX = curGhost.getX();
        int ghostY = curGhost.getY();
        int catX = cat[CatIdle].getX();
        int catY = cat[CatIdle].getY();
        double dist = distance(ghostX, ghostY, catX, catY);

        if (dist > 100 && !curGhost.isDead()) {
            curGhost.GhostMoving(catX, catY);
        }
        else {
            curGhost.setAttacking();
            curGhost.setCurrentFrame(0);

            cat[CatIdle].setHealth(cat[CatIdle].getHealth() - 1);
            isCatHurt = true;
            isCatAttacking = false; // Bị đánh thì hủy đòn đánh
            catHurtStartTime = SDL_GetTicks();
            cat[CatHurt].setCurrentFrame(0);

            if (ghostDieSound != NULL) {
                Mix_PlayChannel(-1, ghostDieSound, 0);
            }
        }
    }

    vecGhost.erase(std::remove_if(vecGhost.begin(), vecGhost.end(),
                                  [](Ghost& g) { return g.isDead(); }),
                   vecGhost.end());

    // Cập nhật Animation Mèo
    if (isCatHurt) {
        cat[CatHurt].update();
        if (SDL_GetTicks() - catHurtStartTime > 400) {
            isCatHurt = false;
        }
    } else if (isCatAttacking) {
        cat[CatAttack].update();
        if (SDL_GetTicks() - attackStartTime > 400) {
            isCatAttacking = false;
        }
    } else if (cat[CatIdle].getHealth() > 0) {
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
    if (GameState == GameOpenning) {
        if (isHoveringPlay) {
            render(GameTexture[Menu1_2Texture]);
        } else {
            render(GameTexture[Menu1Texture]);
        }
        SDL_RenderPresent(renderer);
        return;
    }

    if (GameState == GamePlaying) {
        render(GameTexture[BackGroundTexture]);

        // Ưu tiên render Mèo bị thương > Mèo đánh > Mèo đứng yên
        if (isCatHurt) {
            render(cat[CatHurt], cat[CatHurt].getCurrentFrame());
        } else if (isCatAttacking) {
            render(cat[CatAttack], cat[CatAttack].getCurrentFrame());
        } else {
            render(cat[CatIdle], cat[CatIdle].getCurrentFrame());
        }

        for (auto &curGhost : vecGhost) {
            render(curGhost, curGhost.getCurrentFrame());
        }

        int hx = 20, hy = 20;
        for (int i = 1; i <= CatHealth; i++) {
            heart.setX(hx);
            heart.setY(hy);
            if (i <= cat[CatIdle].getHealth()) render(heart, heart.getFrame(1));
            else render(heart, heart.getFrame(0));
            hx += heart.getWidth() + 20;
        }

        SDL_RenderPresent(renderer);
        return;
    }

    if (GameState == GameWinning) {
        render(GameTexture[Menu2Texture]);
        render(home, home.getCurrentFrame());
        render(again, again.getCurrentFrame());
    }

    if (GameState == GameLosing) {
        render(GameTexture[Menu3Texture]);
        render(home, home.getCurrentFrame());
        render(again, again.getCurrentFrame());
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
