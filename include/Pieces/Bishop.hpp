#ifndef BISHOP_HPP
#define BISHOP_HPP

#include "Piece.hpp"

class Bishop : public Piece {
  public:
    Bishop(Team t_team, Move::BoardPos t_position, sf::Sprite &t_sprite);
    ~Bishop() override = default;

    auto getMoves(std::function<Piece *(Move::BoardPos)> hasPiece, Move::BoardPos board_size) -> MoveList & override;
};

#endif // BISHOP_HPP