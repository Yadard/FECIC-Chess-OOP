#ifndef PAWN_HPP
#define PAWN_HPP

#include "Piece.hpp"

class Pawn : public Piece {
  public:
    Pawn(Team t_team, Move::BoardPos t_position, sf::Sprite &t_sprite);
    ~Pawn() override = default;

    auto getMoves(std::function<Piece *(Move::BoardPos)> hasPiece, Move::BoardPos board_size) -> MoveList & override;

  private:
    Move::BoardPos start_pos;
};

#endif // PAWN_HPP