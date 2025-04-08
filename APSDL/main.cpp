#include "gameplay.h"

using namespace std;

int main(int argc, char *argv[]) {

    Game game;
    game.init();

    while(game.quit == false) {
        game.prepare();
        if(game.Play) game.play();
    }

    game.close();

    return 0;
}