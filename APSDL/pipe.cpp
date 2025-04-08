#include "pipe.h"

void PIPE::load(Graphics g) {
    pipeUpper = g.loadTexture(PIPE_UPPER.c_str());
    pipeUnder = g.loadTexture(PIPE_UNDER.c_str());
}

void PIPE::render( int x, int y, Graphics g, int type) {
    g.renderTextureEx((type == UPPER) ? pipeUpper : pipeUnder, x, y, 0);
}

// scroll backwards
void PIPE::scroll(int distance) {
    upper.x -= distance;
    under.x -= distance;
}

// set position
void PIPE::setPos(int PosX, int PosY) {
    upper.x = under.x = PosX;

    // image : 52 x 320 -> -320 + 100
    upper.y = PosY + (-220);
    under.y = PosY + 270;
}

// check outside
bool PIPE::OutOfScreen() {
    // image : 52 x 320
    if(upper.x + 52 < 0) return true;
    return false;
}

// clear memory
void PIPE::clear() {
    SDL_DestroyTexture(pipeUpper);
    SDL_DestroyTexture(pipeUnder);
}

int PIPE::getX() {
    return (int) upper.x;
}

int PIPE::getY_Upper() {
    return (int) upper.y;
}

int PIPE::getY_Under() {
    return (int) under.y;
}

