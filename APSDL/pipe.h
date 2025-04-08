#ifndef _PIPE__H
#define _PIPE__H

#include "defs.h"
#include "graphics.h"

using namespace std;

class PIPE {
private:
    string PIPE_UPPER = "image/pipe/upper.png";
    string PIPE_UNDER = "image/pipe/under.png";

    SDL_Texture *pipeUpper, *pipeUnder;
    SDL_Rect upper, under;

public:
    void load(Graphics g);
    void render( int x, int y, Graphics g, int type);
    void scroll(int distance);

    void setPos(int PosX, int PosY);
    bool OutOfScreen();

    int getX();
    int getY_Under();
    int getY_Upper();

    void clear();
};

#endif
