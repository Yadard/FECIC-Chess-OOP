#include "Game.hpp"
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>

int main() {
    Game game({1024, 960}, "Chess");

    game.loadAssets();

    game.gameloop();
}