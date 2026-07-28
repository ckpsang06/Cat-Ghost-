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
        std::cout << "Failed to initialize SDL: " << SDL_GetError() << "\n";
    }
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) {
        std::cout << "Failed to initialize SDL_image: " << IMG_GetError() << "\n";
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "Mix_OpenAudio Error: " << Mix_GetError() << "\n";
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
    GameTexture[Menu1Texture] = loadTexture("res/images/menu1.png");
    GameTexture[Menu1_2Texture] = loadTexture("res/images/menu1_2.png");
    GameTexture[CatTexture] = loadTexture("res/images/main.png");
    GameTexture[GhostTexture] = loadTexture("res/images/ghost.png");
    GameTexture[HeartTexture] = loadTexture("res/images/heart.png");


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
    cat[CatIdle].init(GameTexture[CatTexture], 100, 100, 20, 30);
    cat[CatIdle].setHealth(CatHealth);
    cat[CatIdle].setX(SCREEN_WIDTH / 2);
    cat[CatIdle].setY(SCREEN_HEIGHT / 2);

    heart.init(GameTexture[HeartTexture], 50, 50, 2, 0);
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

        // Chỉ xử lý logic chuột khi đang ở màn hình Menu
        if (GameState == GameOpenning) {
            // Lấy tọa độ hiện tại của chuột
            SDL_GetMouseState(&mouseX, &mouseY);

            // ĐỊNH NGHĨA VÙNG NÚT TAM GIÁC (Bounding Box)
            SDL_Rect playButton = { 600, 80, 270, 356 }; // x, y, width, height

            // Kiểm tra xem chuột có nằm trong vùng nút không
            bool isInside = (mouseX >= playButton.x && mouseX <= playButton.x + playButton.w &&
                             mouseY >= playButton.y && mouseY <= playButton.y + playButton.h);

            // 1. Sự kiện di chuyển chuột (Hover)
            if (event.type == SDL_MOUSEMOTION) {
                isHoveringPlay = isInside;
            }

            // 2. Sự kiện click chuột trái
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT && isInside) {
                    GameState = GamePlaying; // Chuyển sang màn chơi
                }
            }

            // có thể nhấn enter để bắt đầu màn chơi
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                GameState = GamePlaying;
            }
        }
    }
}
void Game::gameUpdate() {
    if (GameState != GamePlaying) return;

    //Spawn ma theo thời gian
    if (SDL_GetTicks() - spawnGhostTime > spawnGhostDelay) {
        spawnGhost();
        spawnGhostTime = SDL_GetTicks();
    }

    //Kiểm tra máu mèo
    if (cat[CatIdle].getHealth() <= 0) {
        Mix_HaltMusic();
        // Tạm thời chuyển thẳng về menu hoặc xử lý logic GameEnding ở đây
        GameState = GameOpenning;// Duy Ếch : t đang để thua là trở về gameOpenning, thay gameOpenning thành GameLose khi hết máu nhé
        return;
    }

    //Cập nhật ma và xử lý tương tác
    for (int i = 0; i < (int)vecGhost.size(); i++) {
        auto &curGhost = vecGhost[i];

        int ghostX = curGhost.getX();
        int ghostY = curGhost.getY();
        int catX = cat[CatIdle].getX();
        int catY = cat[CatIdle].getY();

        // Tính khoảng cách từ ma tới mèo
        double dist = distance(ghostX, ghostY, catX, catY);

        // Nếu ở xa và chưa chết  Bay tới chỗ mèo
        if (dist > 100 && !curGhost.isDead()) {
            curGhost.GhostMoving(catX, catY);
        }

        // Nếu ở gần (vào tầm đánh) và chưa chết -> Tấn công
        // Duy Ếch làm hàm dưới này
//        else if (!curGhost.isDead()) {
//
//
//        }

    }

    //Cập nhật animation của mèo
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
    if (GameState == GameOpenning) {
        // Đổi hình ảnh menu dựa vào trạng thái hover chuột
        if (isHoveringPlay) {
            render(GameTexture[Menu1_2Texture]); // Render menu phát sáng
        } else {
            render(GameTexture[Menu1Texture]);   // Render menu bình thường
        }

        SDL_RenderPresent(renderer);
        return;
    }

    if (GameState == GamePlaying) {
        // Vẽ background
        render(GameTexture[BackGroundTexture]);

        // Vẽ Cat đang đứng yên (Idle)
        render(cat[CatIdle], cat[CatIdle].getCurrentFrame());
        for (auto &curGhost : vecGhost) {
            render(curGhost, curGhost.getCurrentFrame());
        }

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
        return;
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
