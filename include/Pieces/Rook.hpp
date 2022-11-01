#ifndef ROOK_HPP
#define ROOK_HPP

#include "Piece.hpp"

class Rook : public Piece {
  public:
    Rook(Team t_team, Move::BoardPos t_position, sf::Sprite &t_sprite);
    ~Rook() override = default;

    auto getMoves(std::function<Piece *(Move::BoardPos)> hasPiece, Move::BoardPos board_size) -> MoveList & override;
};

#endif // ROOK_HPP