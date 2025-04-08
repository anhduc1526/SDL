#include "bird.h"

void Bird::load(Graphics& graphics) {
     Up = graphics.loadTexture(path[0].c_str());
     Mid = graphics.loadTexture(path[1].c_str());
     Down = graphics.loadTexture(path[2].c_str());
}

void Bird::render(const Bird& bird, Graphics& graphics) {
    ++id;
    if(id % 60 <= 20)
        graphics.renderTextureEx(Up, bird.x, bird.y, angle);
    else if (id % 60 <= 40)
        graphics.renderTextureEx(Mid, bird.x, bird.y, angle);
    else
        graphics.renderTextureEx(Down, bird.x, bird.y, angle);
}

int Bird::getX() {
    return x;
}
int Bird::getY() {
    return y;
}

void Bird::move() {
    y = max(y + dy, 0);
    angle = min(angle + (dy > 0) * 5, 45);

    if(dy > 0) {
        dy += 0.1 * gravity++;
    }
}

void Bird::up() {
    dy = -5;
    angle = -45;
    gravity = 1;
}

void Bird::down() {
    dy = 5;
}

// set to initial position
void Bird::reset() {
    x = SCREEN_WIDTH / 7;
    y = SCREEN_HEIGHT / 2;
    angle = 0;
    id = 0;
    gravity = 1;
}

void Bird::setAngle(int newangle) {
    angle = newangle;
}

void Bird::clear() {
    SDL_DestroyTexture(Up);
    SDL_DestroyTexture(Mid);
    SDL_DestroyTexture(Down);
}
