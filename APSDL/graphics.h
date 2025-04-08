#ifndef _GRAPHICS__H
#define _GRAPHICS__H

#include "defs.h"

using namespace std;

class ScrollingBackground {
public:
    SDL_Texture *texture;
    int scrollingOffset = 0;
    int width, height;
    void setTexture(SDL_Texture *_texture) {
        texture = _texture;
        SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    }
    void scroll(int distance) {
        scrollingOffset -= distance;
        if (scrollingOffset < 0) scrollingOffset = width;
    }
};

class Graphics {
private:
    SDL_Renderer *renderer;
    SDL_Window *window;
public:
    void init();
    void logErrorAndExit(const char *msg, const char *error);
    SDL_Texture *loadTexture(const char *path);
    void prepareScene();
    void presentScene();
    void renderTextureEx(SDL_Texture *texture, int x, int y, int angle);

    void renderScrolling(const ScrollingBackground& bgr, int h);

    Mix_Chunk *loadSound(const char *path);
    void playSound(Mix_Chunk *gChunk);
    Mix_Music *loadMusic(const char *path);
    void playMusic(Mix_Music *gMusic);
    void pauseMusic();

    void quit();
};

#endif // _GRAPHICS__H


