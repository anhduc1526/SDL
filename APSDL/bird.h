#ifndef _BIRD__H
#define _BIRD__H

#include "defs.h"
#include "graphics.h"

using namespace std;

class Bird{
private:
    SDL_Texture *Down, *Mid, *Up;
    vector<string> path = {"image/bird/up.png", "image/bird/mid.png", "image/bird/down.png"};
    int x = SCREEN_WIDTH / 7;
    int y = SCREEN_HEIGHT / 2;

    int gravity;
    int angle;
    int dy;
    int id;

public:
    int getX();
    int getY();

    void move();
    void up();
    void down();

    void reset();
    void setAngle(int newangle);

    void load(Graphics& graphics);
    void render(const Bird& bird, Graphics& graphics);
    void clear();
};

#endif

