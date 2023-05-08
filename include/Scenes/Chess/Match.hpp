#ifndef MATCH_H
#define MATCH_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class Piece;

enum class Team {
    WHITE, // Start the game, tradicionally the white pieces
    BLACK  // Goes last in the game, tradicionally the black pieces
};

struct Match {
    virtual auto win(Team) -> void = 0;
    virtual auto createPiece(std::string, Team, sf::Vector2u) -> Piece * {
        std::cout << "FUICKKC" << std::endl;
        return nullptr;
    }
};

#endif // MATCH_H