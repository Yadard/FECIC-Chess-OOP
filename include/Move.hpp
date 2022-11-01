#ifndef MOVE_HPP
#define MOVE_HPP

class Piece;
#include <SFML/Graphics.hpp>

class Move {
  public:
    using BoardPos = sf::Vector2i;
    Move(Piece *t_piece, BoardPos t_destination, BoardPos t_origin);

    auto getPiece() -> Piece *;
    auto getMoveDestination() -> BoardPos;
    auto getMoveOrigin() -> BoardPos;

  private:
    Piece *_piece;
    BoardPos _destination;
    BoardPos _origin;
};

#endif // MOVE_HPP