#ifndef MATCH_H
#define MATCH_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class Piece;

enum class Team { WHITE, BLACK, UNKNOWN };

struct Match {
    virtual auto win(Team) -> void = 0;
    virtual auto createPiece(std::string, Team, sf::Vector2u) -> Piece * = 0;
    virtual auto addPiece(Piece *, sf::Vector2u) -> void = 0;
    virtual auto hasPiece(sf::Vector2u) const -> Piece * = 0;
    virtual auto getBoardSize() const -> sf::Vector2u = 0;
    virtual auto endTurn() -> void = 0;
};

#endif // MATCH_H