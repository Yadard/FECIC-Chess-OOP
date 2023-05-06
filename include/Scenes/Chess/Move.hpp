#ifndef MOVE_HPP
#define MOVE_HPP

class Piece;
#include <SFML/Graphics.hpp>

class Move {
  public:
    using BoardPos = sf::Vector2u;
    Move(Piece *t_piece, BoardPos t_destination, BoardPos t_origin);

    auto getPiece() const -> Piece *;
    auto getMoveDestination() const -> BoardPos;
    auto getMoveOrigin() const -> BoardPos;

  private:
    Piece *_piece;
    BoardPos _destination;
    BoardPos _origin;
};

#endif // MOVE_HPP