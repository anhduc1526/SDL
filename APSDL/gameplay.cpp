#include "gameplay.h"

void Game::init() {
    graphics.init();
    tPipe.load(graphics);

    // load pipes
    for(int i = 0; i < 4; ++i) {
        tPipe.setPos(SCREEN_WIDTH + PIPE_DISTANCE * i, rand() % (LAND_HEIGHT + 1));
        pipe.push_back(tPipe);
    }

    // load numbers' assets
    smallNum.resize(10);
    largeNum.resize(10);
    for(int i = 0; i < 10; ++i) {
        string tmp;
        tmp = "image/bignum/" + to_string(i) + ".png";
        largeNum[i] = (graphics.loadTexture(tmp.c_str()));
        tmp = "image/smallnum/" + to_string(i) + ".png";
        smallNum[i] = graphics.loadTexture(tmp.c_str());
    }

    // load assets
    Left = graphics.loadTexture("image/button/left.png");
    Right = graphics.loadTexture("image/button/right.png");
    Sound = graphics.loadTexture("image/button/soundon.png");
    NotSound = graphics.loadTexture("image/button/soundoff.png");
    replay = graphics.loadTexture("image/button/replay.png");
    State = graphics.loadTexture("image/button/state.png");

    message = graphics.loadTexture("image/message/message.png");
    gameOver = graphics.loadTexture("image/message/gameOver.png");
    pauseMessage = graphics.loadTexture("image/message/pauseMessage.png");

    land.setTexture(graphics.loadTexture("image/background/land.png"));
    bgrr.setTexture(graphics.loadTexture("image/background/day.png"));
    bgrrN.setTexture(graphics.loadTexture("image/background/night.png"));

    // load audio
    sMusic = graphics.loadMusic("audio/music.mp3");
    sClick = graphics.loadSound("audio/click.mp3");
    sFlap = graphics.loadSound("audio/flap.mp3");
    sDead = graphics.loadSound("audio/dead.mp3");
    sPoint = graphics.loadSound("audio/point.wav");

    graphics.playMusic(sMusic);
    bird.load(graphics);

    // load high score
    fstream file("score.txt");
    file >> bestScore;
    file.close();

    // game status
    Play = score = die = quit = false;
    isDay = isSound = true;
}

void Game::close() {
    bird.clear();
    for (PIPE i : pipe) i.clear();
    for (SDL_Texture *i : smallNum) SDL_DestroyTexture(i);
    for (SDL_Texture *i : largeNum) SDL_DestroyTexture(i);

    SDL_DestroyTexture(Left);
    SDL_DestroyTexture(Right);
    SDL_DestroyTexture(Sound);
    SDL_DestroyTexture(NotSound);
    SDL_DestroyTexture(replay);
    SDL_DestroyTexture(State);

    SDL_DestroyTexture(message);
    SDL_DestroyTexture(gameOver);
    SDL_DestroyTexture(pauseMessage);

    Mix_FreeChunk(sClick);
    Mix_FreeChunk(sFlap);
    Mix_FreeChunk(sDead);
    Mix_FreeChunk(sPoint);
    Mix_FreeMusic(sMusic);

    graphics.quit();
}

bool Game::collision(PIPE cur) {
    // check collision with pipes
    // pipe size : 52 x 320
     if (cur.getX() - 29 <= bird.getX() && bird.getX() <= cur.getX() + 52) {
         if (cur.getY_Upper() - 24 <= bird.getY() && bird.getY() <= cur.getY_Upper() + 320) return true;
         if (cur.getY_Under() - 24 <= bird.getY() && bird.getY() <= cur.getY_Under() + 320) return true;
     }

    // touch land
    if (bird.getY() >= 475) return true;

    return false;
}

void Game::updateHighScore() {
    bestScore = score;
    fstream fin("score.txt");
    fin << bestScore;
    fin.close();
}

void Game::renderLargeNum(int score) {
    string s = to_string(score);
    // image : 30 x 45
    int width = 30;
    int PosX = (SCREEN_WIDTH - width * (int) s.size()) / 2;
    for (char c : s) {
        graphics.renderTextureEx(largeNum[c - '0'], PosX, 10, 0);
        PosX += width;
    }
}

void Game::renderSmallNum(int score, int posX, int posY) {
    string s = to_string(score);
    // image : 21 x 30
    int width = 21;
    int currentPosX = posX - width * s.length();
    for (char c : s) {
        graphics.renderTextureEx(smallNum[c - '0'], currentPosX, posY, 0);
        currentPosX += width;
    }
}

void Game::updateMusicAndSound() {
    if (isSound) graphics.pauseMusic();
    else graphics.playMusic(sMusic);
    isSound ^= 1;
}

void Game::prepare() {
    die = false;

    bgrr.scroll(4);
    bgrrN.scroll(4);
    land.scroll(4);
    graphics.renderScrolling(isDay ? bgrr : bgrrN, 0);
    graphics.renderScrolling(land, 500);

    // Day - Night
    graphics.renderTextureEx(Left, 0, SCREEN_HEIGHT / 2, 0);
    graphics.renderTextureEx(Right, SCREEN_WIDTH - 13, SCREEN_HEIGHT / 2, 0);

    graphics.renderTextureEx(message, (SCREEN_WIDTH - MESSAGE_WIDTH) / 2, (SCREEN_HEIGHT - MESSAGE_HEIGHT - LAND_HEIGHT) / 2, 0);
    graphics.renderTextureEx(isSound ? Sound : NotSound, 10, 10, 0);

    bird.reset();
    bird.render(bird, graphics);

    graphics.presentScene();

    SDL_Event e;

    if (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) die = quit = true;

        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) Play = true;

        if (e.type == SDL_MOUSEBUTTONDOWN) {
            SDL_GetMouseState(&mouseX, &mouseY);
            cerr << mouseX << ' ' << mouseY << '\n';
            // image : 32 x 24

            if (mouseX >= 10 && mouseX <= 10 + 32 && mouseY >= 10 && mouseY <= 10 + 24) {
                updateMusicAndSound();
                if (isSound) graphics.playSound(sClick);
            }

            // image : 13 x 16
            // 0, 312
            else if (mouseX >= 0 && mouseX <= 0 + 13 && mouseY >= 312 && mouseY <= 312 + 16) {
                isDay ^= 1;
                if (isSound) graphics.playSound(sClick);
            }

            // image : 13 x 16
            // 690, 315
            else if (mouseX >= 690 && mouseX <= 690 + 13 && mouseY >= 312 && mouseY <= 312 + 16) {
                isDay ^= 1;
                if (isSound) graphics.playSound(sClick);
            }

            else Play = true;
        }
    }
}

void Game::update() {
    graphics.prepareScene();
    bgrr.scroll(4);
    bgrrN.scroll(4);
    graphics.renderScrolling(isDay ? bgrr : bgrrN, 0);

    bird.move();
    bird.render(bird,graphics);

    for (auto &i : pipe) {
        i.scroll(4);
        i.render(i.getX(), i.getY_Upper(), graphics, UPPER);
        i.render(i.getX(), i.getY_Under(), graphics, UNDER);
    }

    if (pipe[0].OutOfScreen()) {
        pipe.erase(pipe.begin());
        tPipe.setPos(pipe[2].getX() + PIPE_DISTANCE, rand() % (LAND_HEIGHT + 1));
        pipe.push_back(tPipe);
    }

    renderLargeNum(score);
    // 630 20
    graphics.renderTextureEx(State, 630, 20, 0);

    land.scroll(4);
    graphics.renderScrolling(land, 500);

    graphics.presentScene();
}

void Game::play() {
    const int TARGET_FPS = 60;
    Uint32 frameStart, frameTime;
    SDL_Event event;

    while(!die) {
        if (collision(pipe[0])) {
            if (score > bestScore) updateHighScore();
            dead();
            continue;
        }

        if (pipe[0].getX() == bird.getX()) {
            ++score;
            if (isSound) graphics.playSound(sPoint);
        }

        frameStart = SDL_GetTicks();

        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                die = quit = true;
            }

            const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

            if (currentKeyStates[SDL_SCANCODE_SPACE] || event.type == SDL_MOUSEBUTTONDOWN) {
                bird.up();
                if (isSound) graphics.playSound(sFlap);
            }
            else bird.down();

            // pause game
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                SDL_GetMouseState(&mouseX, &mouseY);
                // image : 50 x 50
                if (mouseX >= 630 && mouseX <= 630 + 50 && mouseY >= 20 && mouseY <= 20 + 50) {
                    if (isSound) graphics.playSound(sClick);
                    pause();
                }
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) pause();
        }

        update();
        frameTime = SDL_GetTicks() - frameStart;

        if (frameTime < 1000 / TARGET_FPS) {
            SDL_Delay((1000 / TARGET_FPS) - frameTime);
        }
    }
}

void Game::pause() {
    SDL_Event event;
    while(true) {
        graphics.prepareScene();

        graphics.renderScrolling(isDay ? bgrr : bgrrN, 0);
        for (auto &i : pipe) {
            // i.scroll(2);
            i.render(i.getX(), i.getY_Upper(), graphics, UPPER);
            i.render(i.getX(), i.getY_Under(), graphics, UNDER);
        }

        renderLargeNum(score);
        graphics.renderScrolling(land, 500);
        bird.render(bird, graphics);

        // 630 20
        graphics.renderTextureEx(State, 630, 20, 0);
        // image : 400 x 40
        const int Offset = 260;
        graphics.renderTextureEx(pauseMessage, SCREEN_WIDTH / 2 - 400 / 2, SCREEN_HEIGHT / 2 - 40 / 2 + Offset, 0);

        graphics.presentScene();

        if (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = die = true;
                break;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                SDL_GetMouseState( &mouseX, &mouseY);
                // image : 50 x 50
                if (mouseX >= 630 && mouseX <= 630 + 50 && mouseY >= 20 && mouseY <= 20 + 50) {
                    if (isSound) graphics.playSound(sClick);
                    break;
                }
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) break;
        }
    }
}


void Game::dead() {
    if (isSound) graphics.playSound(sDead);

    // Game over image
    graphics.renderTextureEx(gameOver, (SCREEN_WIDTH - GAMEOVER_WIDTH) / 2, (500 - GAMEOVER_HEIGHT) / 2, 0);
    graphics.presentScene();

    // Best score, current score is above
    renderSmallNum(score, 370, 250);
    renderSmallNum(bestScore, 370, 300);

    // Replay button
    graphics.renderTextureEx(replay, (SCREEN_WIDTH - 100) / 2, 360, 0);
    graphics.presentScene();

    die = true;

    SDL_Event event;
    while (true) {
        if (SDL_PollEvent(&event) != 0) {

            if (event.type == SDL_QUIT) {
                quit = true;
                break;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                SDL_GetMouseState(&mouseX, &mouseY);
                cerr << mouseX << ' ' << mouseY << '\n';
                // image : 100 x 56
                // pos : 303, 363
                if (mouseX >= 300 && mouseX <= 300 + 100 && mouseY >= 360 && mouseY <= 360 + 56) {
                    if (isSound) graphics.playSound(sClick);
                    reset();
                    Play = false;
                    break;
                }
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                if (isSound) graphics.playSound(sClick);
                reset();
                Play = false;
                break;
                Play = false;
            }
        }
    }
};

void Game::reset() {
    bird.reset();
    pipe.clear();
    for(int i = 0; i < 4; ++i) {
        tPipe.setPos(500 + PIPE_DISTANCE * i, rand() % (LAND_HEIGHT + 1));
        pipe.push_back(tPipe);
    }
    score = 0;
}
