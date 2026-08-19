    #include<string>
    #include "Game.h"
    #include "CommonFunc.h"

    using namespace std;

    Game::Game() {
        isRunning = true;
        window = NULL;
        renderer = NULL;
        PlayingMusic = NULL;
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
        if (TTF_Init() == -1) {
            cout << "Failed to initialize SDL_ttf: " << TTF_GetError() << "\n";
        }
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            std::cout << "Failed to initialize SDL_mixer: " << Mix_GetError() << "\n";
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
        GameTexture[CatRunningTexture] = loadTexture("res/images/running.png");
        GameTexture[CatDieTexture] = loadTexture("res/images/die.png");
        GameTexture[CatVictoryTexture] = loadTexture("res/images/victory.png");
        GameTexture[CatMagicTexture] = loadTexture("res/images/sunken.png");
        GameTexture[TornadoTexture] = loadTexture("res/images/wingar_displayed.png");
        GameTexture[SpellTitleTexture] = loadTexture("res/images/Wingardium Leviosa Tittle.png");
        GameTexture[CatLightningTexture] = loadTexture("res/images/lightning.png");
        GameTexture[LightningStrikeTexture] = loadTexture("res/images/lightning_attack.png");
        GameTexture[LightningTexture] = loadTexture("res/images/lightning_displayed.png");
        GameTexture[Spell2TitleTexture] = loadTexture("res/images/Expecto Patronum Tittle.png");
        //fonts
        font = TTF_OpenFont("res/fonts/arial.ttf", 50);
        TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
        if (!font) {
            cout << "Failed to load font: " << TTF_GetError();
        }

        //musics
        PlayingMusic = Mix_LoadMUS("res/sounds/backgroundsound.mp3");
        ghostDieSound = Mix_LoadWAV("res/sounds/die.ogg");
        windSound = Mix_LoadWAV("res/sounds/sound_wind.ogg");
        spellVoiceSound = Mix_LoadWAV("res/sounds/wingdardium_leviosa.mp3");
        spell2VoiceSound = Mix_LoadWAV("res/sounds/expecto_patronum.mp3");
        thunderSound = Mix_LoadWAV("res/sounds/thunder.ogg");
    }

    void Game::initEntity() {
        cat[CatIdle].init(GameTexture[CatTexture], 100, 100, 20, 80); // Đã update delay animation
        cat[CatIdle].setHealth(CatHealth);
        cat[CatIdle].setX(SCREEN_WIDTH / 2);
        cat[CatIdle].setY(SCREEN_HEIGHT / 2);

        cat[CatHurt].init(GameTexture[CatHurtTexture], 200, 200, 4, 50);
        cat[CatHurt].setX(cat[CatIdle].getX() - 50);
        cat[CatHurt].setY(cat[CatIdle].getY() - 60);

        cat[CatAttack].init(GameTexture[CatAttackTexture], 200, 200, 6, 40);
        cat[CatAttack].setX(cat[CatIdle].getX() - 50);
        cat[CatAttack].setY(cat[CatIdle].getY() - 80);

        cat[CatRunning].init(GameTexture[CatRunningTexture], 200, 200, 6, 40);
        cat[CatRunning].setX(-120);
        cat[CatRunning].setY(cat[CatIdle].getY() - 80);

        cat[CatDie].init(GameTexture[CatDieTexture], 150, 150, 10, 200);
        cat[CatDie].setX(SCREEN_WIDTH / 2 - 40);
        cat[CatDie].setY(SCREEN_HEIGHT / 2 - 40);

        cat[CatVictory].init(GameTexture[CatVictoryTexture], 200, 200, 36, 80);
        cat[CatVictory].setX(SCREEN_WIDTH / 2 - 50);
        cat[CatVictory].setY(SCREEN_HEIGHT / 2 - 100);

        cat[CatMagic].init(GameTexture[CatMagicTexture], 200, 200, 8, 1500 / 8);
        cat[CatMagic].setX(cat[CatIdle].getX() - 50);
        cat[CatMagic].setY(cat[CatIdle].getY() - 80);

        spellTitle.init(GameTexture[SpellTitleTexture], 300, 50, 1, 0);
        spellTitle.setX(-300);
        spellTitle.setY(80);

        tornadoIcon.init(GameTexture[TornadoTexture], 50, 50, 9, 100);


        heart.init(GameTexture[HeartTexture], 50, 50, 2, 0);

        home.init(GameTexture[HomeTexture], 140, 260, 2, 0);
        home.setX(702);
        home.setY(461);

        again.init(GameTexture[AgainTexture], 140, 140, 2, 0);
        again.setX(930);
        again.setY(518);

        catLightning.init(GameTexture[CatLightningTexture], 200, 200, 8, 60);
        catLightning.setX(cat[CatIdle].getX() - 50);
        catLightning.setY(cat[CatIdle].getY() - 80);

        lightningStrike.init(GameTexture[LightningStrikeTexture], 400, 400, 3, 100);
        lightningStrike.setX(SCREEN_WIDTH / 2 - 200);
        lightningStrike.setY(SCREEN_HEIGHT / 2 - 250);

        spell2Title.init(GameTexture[Spell2TitleTexture], 300, 50, 1, 0);
        spell2Title.setX(-300);
        spell2Title.setY(80);

        lightningIcon.init(GameTexture[LightningTexture], 80, 80, 6, 100);
    }
    void Game::resetGame() {
        //Reset thời gian và thời gian chờ sinh quái
        spawnGhostTime = SDL_GetTicks();
        spawnGhostDelay = spawnGhostDelayBeforeCatRunning;

        //Reset máu và tọa độ cho CatIdle (vị trí trung tâm)
        cat[CatIdle].setHealth(CatHealth);
        cat[CatIdle].setX(SCREEN_WIDTH / 2);
        cat[CatIdle].setY(SCREEN_HEIGHT / 2);

        //Đặt tọa độ ban đầu cho CatRunning (chạy từ ngoài mép trái vào)
        cat[CatRunning].setX(-120);
        cat[CatRunning].setY(cat[CatIdle].getY() - 80);

        //Reset toàn bộ frame animation về 0 để không bị kẹt ảnh cũ
        cat[CatRunning].setCurrentFrame(0);
        cat[CatDie].setCurrentFrame(0);
        cat[CatVictory].setCurrentFrame(0);
        cat[CatIdle].setCurrentFrame(0);
        cat[CatAttack].setCurrentFrame(0);
        cat[CatHurt].setCurrentFrame(0);

        isAltPressed = false;
        isMagicReady = true;
        cat[CatMagic].setCurrentFrame(0);

        //Reset các cờ trạng thái và điểm số
        isCatHurt = false;
        isUltimateActive = false;
        isChargingUltimate = false;
        // Trừ đi 10s để khi mới vào game là có thể bấm Space dùng chiêu được ngay
        lastUltimateTime = SDL_GetTicks() - 10000;
        catLightning.setCurrentFrame(0);
        lightningStrike.setCurrentFrame(0);
        isCatAttacking = false;
        SCORE = 0;

        // Xóa sạch quái vật từ ván chơi trước
        vecGhost.clear();
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

            // MÀN HÌNH MENU
            if (GameState == GameOpenning) {
                SDL_GetMouseState(&mouseX, &mouseY);
                SDL_Rect playButton = { 600, 80, 270, 356 };

                bool isInside = (mouseX >= playButton.x && mouseX <= playButton.x + playButton.w &&
                                 mouseY >= playButton.y && mouseY <= playButton.y + playButton.h);

                if (event.type == SDL_MOUSEMOTION) {
                    isHoveringPlay = isInside;
                }

                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && isInside) {
                    GameState = GameIntro;
                    Mix_PlayMusic(PlayingMusic, -1);
                    resetGame();
                }

            }

            // MÀN HÌNH CHƠI
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

                        // Kiểm tra click trúng con Ma nào thì con đó chết
                        int clickX = event.button.x;
                        int clickY = event.button.y;
                        for (auto &curGhost : vecGhost) {
                                if (curGhost.isDead()) continue;
                                int gx = (int)curGhost.getX();
                                int gy = (int)curGhost.getY();
                                int gw = curGhost.getWidth();
                                int gh = curGhost.getHeight();
                                if (clickX >= gx && clickX <= gx + gw &&
                                    clickY >= gy && clickY <= gy + gh) {
                                            curGhost.setDead();
                                            SCORE++;
                                            if (ghostDieSound != NULL)
                                                Mix_PlayChannel(-1, ghostDieSound, 0);
                                            break; // Mỗi click chỉ giết đúng 1 con
                                    }
                        }
                    }
                }
                // Kích hoạt sấm sét bằng phím space
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                    // Điều kiện: Không bị thương, không đang dùng chiêu, và đã hồi chiêu xong (10 giây)
                    if (!isCatHurt && !isUltimateActive && isMagic2Ready && ! isSpacePressed) {
                        isSpacePressed = true;
                        SpacePressStartTime = SDL_GetTicks();
                        //Set lại vị trí spell title sau mỗi lần gọi
                        spell2Title.setX(-300);
                        cat[CatAttack].setCurrentFrame(0);
                        if (spell2VoiceSound != NULL) {
                            Mix_PlayChannel(-1, spell2VoiceSound, 0);
                        }
                    }
                }
                //Xử lý giữ nút SPACE để gồng ma thuật
                if (event.type == SDL_KEYUP) {
                    if (event.key.keysym.sym == SDLK_SPACE) {
                        isSpacePressed = false  ;
                    }
                }

                // Xử lý giữ nút ALT để gồng ma thuật
                if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_LALT || event.key.keysym.sym == SDLK_RALT) {
                        if (!isCatHurt && !isCatAttacking && isMagicReady && !isAltPressed) {
                            isAltPressed = true;
                            altPressStartTime = SDL_GetTicks();

                            // Set lại vị trí spell title sau mỗi lần gọi
                            spellTitle.setX(-300);
                            cat[CatMagic].setCurrentFrame(0); // Bắt đầu chạy animation gồng
                            if (spellVoiceSound != NULL) {
                                Mix_PlayChannel(-1, spellVoiceSound, 0);
                            }
                        }
                    }
                }
                // Hủy gồng nếu nhả nút ALT ra sớm
                if (event.type == SDL_KEYUP) {
                    if (event.key.keysym.sym == SDLK_LALT || event.key.keysym.sym == SDLK_RALT) {
                        isAltPressed = false;
                    }
                }
            }

            // MÀN HÌNH KẾT THÚC (THẮNG / THUA)
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
                        GameState = GameIntro;
                        Mix_PlayMusic(PlayingMusic,-1);
                        resetGame();
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
        if (GameState == GameDie) {
            if (cat[CatDie].getCur() <= 8) {
                cat[CatDie].update();
            }
            if (SDL_GetTicks() - CatDieStartTime >= 2000) {
                GameState = GameLosing;
            }
            return;
        }

        if (GameState == GameVictory) {
            if (cat[CatVictory].getCur() <= 34) {
                cat[CatVictory].update();
            }
            if (SDL_GetTicks() - CatVictoryStartTime >= 2880) {
                GameState = GameWinning;
            }
            return;

        }

        if (GameState == GameIntro) {
            float currentX = cat[CatRunning].getX();
            float targetX = SCREEN_WIDTH / 2 - 50;
            float speed = 5.0f;
            if (currentX < targetX) {
                cat[CatRunning].setX(currentX + speed);
                cat[CatRunning].update();
            } else {
                GameState = GamePlaying;
                gameStartTime = SDL_GetTicks();
                spawnGhostTime = SDL_GetTicks();
                spawnGhostDelay = spawnGhostDelayBeforeCatRunning;
            }
            return;
        }

        if (GameState != GamePlaying) return;

        if (SDL_GetTicks() - gameStartTime >= GAME_TIME) {
            GameState = GameVictory;
            CatVictoryStartTime = SDL_GetTicks();
            Mix_HaltMusic();
            return;
        }
        if (SDL_GetTicks() - spawnGhostTime > spawnGhostDelay) {
            spawnGhost();
            spawnGhostTime = SDL_GetTicks();
            spawnGhostDelay = spawnGhostDelayAfterCatRunning;
        }

        if (cat[CatIdle].getHealth() <= 0) {
            Mix_HaltMusic();
            GameState = GameDie;
            CatDieStartTime = SDL_GetTicks();
            cat[CatDie].setCurrentFrame(0);
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

        // Cập nhật Animation Mèo và Logic Ma thuật
        if (isCatHurt) {
            cat[CatHurt].update();
            if (SDL_GetTicks() - catHurtStartTime > 200) isCatHurt = false;
        }
        else if (isUltimateActive) {
            catLightning.update();
            lightningStrike.update();
            // Animation chiêu cuối kéo dài khoảng 600ms
            if (SDL_GetTicks() - ultimateStartTime > 600) {
                isUltimateActive = false;
                isMagic2Ready = false;
                magic2CooldownStartTime = SDL_GetTicks();
                lightningTargets.clear();
            }
        }
        else if (isSpacePressed && isMagic2Ready) {
            // Tăng vị tri X của spell
            if (spell2Title.getX() < 20) {
                spell2Title.setX(spell2Title.getX() + 10);
            }

            cat[CatAttack].update();
            if (SDL_GetTicks() - SpacePressStartTime >= MAGIC_CHARGE_TIME) {
                isSpacePressed = false;
                isUltimateActive = true;
                ultimateStartTime = SDL_GetTicks();
                catLightning.setCurrentFrame(0);
                lightningStrike.setCurrentFrame(0);
                lightningTargets.clear();
                for (auto &curGhost : vecGhost) {
                    if (!curGhost.isDead()) {
                        curGhost.setDead();
                        SCORE++;
                        SDL_Rect target;
                        target.x = curGhost.getX() - 125;
                        target.y = curGhost.getY() - 150;
                        lightningTargets.push_back(target);
                    }
                }
                if (thunderSound != NULL) {
                    Mix_PlayChannel(-1, thunderSound, 0);
                }
            }
        }
        else if (isAltPressed && isMagicReady) {
            // Tăng vị trí X của Spell
            if (spellTitle.getX() < 20) {
                spellTitle.setX(spellTitle.getX() + 10);
            }
            cat[CatMagic].update();

            if (SDL_GetTicks() - altPressStartTime >= MAGIC_CHARGE_TIME) {
                if (windSound != NULL) Mix_PlayChannel(-1, windSound, 0);

                for (auto &curGhost : vecGhost) {
                    if (!curGhost.isDead()) {
                        double dx = curGhost.getX() - cat[CatIdle].getX();
                        double dy = curGhost.getY() - cat[CatIdle].getY();
                        double dist = distance(curGhost.getX(), curGhost.getY(), cat[CatIdle].getX(), cat[CatIdle].getY());

                        if (dist > 0) {
                            // Đẩy lùi ra xa thêm 400 pixel
                            curGhost.setX(curGhost.getX() + (dx / dist) * 400);
                            curGhost.setY(curGhost.getY() + (dy / dist) * 400);
                        }
                    }
                }
                isMagicReady = false;
                magicCooldownStartTime = SDL_GetTicks();
                isAltPressed = false; // Ngắt trạng thái gồng
            }
        }
        else if (isCatAttacking) {
            cat[CatAttack].update();
            if (SDL_GetTicks() - attackStartTime > 240) isCatAttacking = false;
        }
        else if (cat[CatIdle].getHealth() > 0) {
            cat[CatIdle].update();
        }

        // Cập nhật Cooldown & Lốc xoáy
        if (!isMagicReady) {
            // Đếm thời gian hồi chiêu
            if (SDL_GetTicks() - magicCooldownStartTime >= MAGIC_COOLDOWN) {
                isMagicReady = true;
            }
        }

        if (!isMagic2Ready) {
            // Đếm thời gian hồi chiêu
            if (SDL_GetTicks() - magic2CooldownStartTime >= MAGIC_COOLDOWN) {
                isMagic2Ready = true;
            }
        }

        if (isMagic2Ready) {
            lightningIcon.update();
        }

        if (isMagicReady) {
            tornadoIcon.update();
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

    void Game::renderText(const std::string &text, int x, int y, SDL_Color color) {
        if (font == NULL) return;
        SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
        if (surface == NULL) return;
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dst = { x, y, surface->w, surface->h };
        SDL_RenderCopy(renderer, texture, NULL, &dst);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    void Game::gameRender() {
        clear();
        if (GameState == GameOpenning) {
            if (isHoveringPlay) {
                render(GameTexture[Menu1_2Texture]);
            } else {
                render(GameTexture[Menu1Texture]);
            }
        }
        else if (GameState == GameIntro) {
            render(GameTexture[BackGroundTexture]);
            render(cat[CatRunning], cat[CatRunning].getCurrentFrame());
        }

        else if (GameState == GameDie) {
            render(GameTexture[BackGroundTexture]);
            render(cat[CatDie],cat[CatDie].getCurrentFrame());
        }

        else if (GameState == GameVictory) {
            render(GameTexture[BackGroundTexture]);
            render(cat[CatVictory],cat[CatVictory].getCurrentFrame());
        }

        else if (GameState == GamePlaying) {
            render(GameTexture[BackGroundTexture]);

            if (isCatHurt) {
                render(cat[CatHurt], cat[CatHurt].getCurrentFrame());
            } else if (isUltimateActive) {
                // Duyệt qua mảng vị trí và vẽ sét
                for (SDL_Rect pos : lightningTargets) {
                    lightningStrike.setX(pos.x);
                    lightningStrike.setY(pos.y);
                    render(catLightning, catLightning.getCurrentFrame());
                    render(lightningStrike, lightningStrike.getCurrentFrame());
                }
            } else if (isAltPressed && isMagicReady) {
                render(cat[CatMagic], cat[CatMagic].getCurrentFrame());
            } else if (isSpacePressed && isMagic2Ready) {
                render(cat[CatAttack], cat[CatAttack].getCurrentFrame());
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

            //Render Dòng chữ Tên chiêu (Nếu đang giữ Alt)
            if (isAltPressed && isMagicReady) {
                render(spellTitle, spellTitle.getCurrentFrame());
            }

            //Render Dòng chữ Tên chiêu (Nếu đang giữ Space)
            if (isSpacePressed && isMagic2Ready) {
                render(spell2Title, spell2Title.getCurrentFrame());
            }

            SDL_Color scoreColor = {218, 165, 32, 255};  // màu vàng
            std::string scoreText = std::to_string(SCORE);
            int textW = 0, textH = 0;
            TTF_SizeText(font, scoreText.c_str(), &textW, &textH);
            int scoreX = SCREEN_WIDTH - textW - 20;
            renderText(scoreText, scoreX, 15, scoreColor);

            if (isMagicReady) {
                tornadoIcon.setX(scoreX - 300);
                tornadoIcon.setY(20);
                render(tornadoIcon, tornadoIcon.getCurrentFrame());
            }

            if (isMagic2Ready) {
                lightningIcon.setX(scoreX - 600);
                lightningIcon.setY(5);
                render(lightningIcon, lightningIcon.getCurrentFrame());
            }
        }

        else if (GameState == GameWinning) {
            render(GameTexture[Menu2Texture]);
            render(home, home.getCurrentFrame());
            render(again, again.getCurrentFrame());
            renderText("YOUR SCORE", 750, 300, yellowColor);
            renderText(to_string(SCORE), 845, 400, yellowColor);
        }

        else if (GameState == GameLosing) {
            render(GameTexture[Menu3Texture]);
            render(home, home.getCurrentFrame());
            render(again, again.getCurrentFrame());
            renderText("YOU LOST", 775, 300, yellowColor);

        }

        SDL_RenderPresent(renderer);
    }

    void Game::cleanUp() {
            if (PlayingMusic != NULL) {
            Mix_FreeMusic(PlayingMusic);
            PlayingMusic = NULL;
        }
        Mix_Quit();
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
