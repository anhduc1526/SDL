#ifndef _GAMEPLAY__H
#define _GAMEPLAY__H

#include <iostream>
#include <vector>
#include <string.h>
#include <fstream>

#include "defs.h"
#include "bird.h"
#include "pipe.h"

using namespace std;

class Game {
private:
    int mouseX, mouseY;

    int isDay, isSound;
    bool die;
    int score, bestScore;

    PIPE tPipe;
    Bird bird;

    Graphics graphics;
    ScrollingBackground land, bgrr, bgrrN;

    vector <PIPE> pipe;
    vector <SDL_Texture*> smallNum, largeNum;

    SDL_Texture *Left, *Right;
    SDL_Texture *Sound;
    SDL_Texture *NotSound;
    SDL_Texture *replay;
    SDL_Texture *State;

    SDL_Texture *message;
    SDL_Texture *gameOver;
    SDL_Texture *pauseMessage;

    Mix_Music *sMusic;
    Mix_Chunk *sClick;
    Mix_Chunk *sFlap;
    Mix_Chunk *sDead;
    Mix_Chunk *sPoint;

public:
    bool Play, quit;

    bool collision(PIPE cur);

    void init();
    void close();

    void updateMusicAndSound();
    void updateHighScore();

    void renderLargeNum(int num);
    void renderSmallNum(int num, int posX, int posY);

    void prepare();
    void play();
    void update();
    void pause();
    void dead();
    void reset();
};

#endif // _GAME__H