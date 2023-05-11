#include "Game.hpp"

int main() {
    Game game({1024, 960}, "Chess");

    game.loadAssets();

    game.gameloop();

    return EXIT_SUCCESS;
}