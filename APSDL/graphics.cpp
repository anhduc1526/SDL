#include "graphics.h"

void Graphics::logErrorAndExit(const char* msg, const char* error) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,"%s: %s", msg, error);
    SDL_Quit();
}

void Graphics::init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) logErrorAndExit("SDL_Init", SDL_GetError());

    window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
    if (window == nullptr) logErrorAndExit("CreateWindow", SDL_GetError());

    if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)) logErrorAndExit( "SDL_image error:", IMG_GetError());

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) logErrorAndExit("CreateRenderer", SDL_GetError());

    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        {
            logErrorAndExit( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_Surface* icon = IMG_Load("image/icon.png");
    SDL_SetWindowIcon(window, icon);
}

void Graphics::prepareScene() {
    SDL_RenderClear(renderer);
}
void Graphics::presentScene() {
    SDL_RenderPresent(renderer);
}

SDL_Texture* Graphics::loadTexture(const char *filename) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);
    SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
    if (texture == NULL) SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_ERROR, "Load texture %s", IMG_GetError());
    return texture;
}
void Graphics::renderTextureEx(SDL_Texture *texture, int x, int y, int angle) {
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;

    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopyEx(renderer, texture, NULL, &dest, angle, NULL, SDL_FLIP_NONE);
}

void Graphics::renderScrolling(const ScrollingBackground& bgr,int h) {
        renderTextureEx(bgr.texture, bgr.scrollingOffset, h, 0);
        renderTextureEx(bgr.texture, bgr.scrollingOffset - bgr.width, h, 0);
}

void Graphics::quit() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Mix_Quit();
    SDL_Quit();
    IMG_Quit();
}

Mix_Chunk* Graphics::loadSound(const char* path) {
        Mix_Chunk* gChunk = Mix_LoadWAV(path);
        if (gChunk == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,"Could not load sound! SDL_mixer Error: %s", Mix_GetError());
        }
    return gChunk;
}

void Graphics::playSound(Mix_Chunk* gChunk) {
        if (gChunk != nullptr) {
            Mix_PlayChannel( -1, gChunk, 0);
        }
}

Mix_Music* Graphics::loadMusic(const char* path){
        Mix_Music *gMusic = Mix_LoadMUS(path);
        if (gMusic == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
                           "Could not load music! SDL_mixer Error: %s", Mix_GetError());
        }
        return gMusic;
}

void Graphics::playMusic(Mix_Music *gMusic){
    if (gMusic == nullptr) return;

        if (Mix_PlayingMusic() == 0) {
            Mix_PlayMusic( gMusic, -1);
        }
        else if( Mix_PausedMusic() == 1) {
            Mix_ResumeMusic();
        }
}

void Graphics::pauseMusic(){
    Mix_PauseMusic();
}

