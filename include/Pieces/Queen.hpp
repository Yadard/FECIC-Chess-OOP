#ifndef QUEEN_HPP
#define QUEEN_HPP

#include "Piece.hpp"

class Queen : public Piece {
  public:
    Queen(Team t_team, Move::BoardPos t_position, sf::Sprite &t_sprite);
    ~Queen() override = default;

    auto getMoves(std::function<Piece *(Move::BoardPos)> hasPiece, Move::BoardPos board_size) -> MoveList & override;
};

#endif // QUEEN_HPP