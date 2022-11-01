#ifndef KNIGHT_HPP
#define KNIGHT_HPP

#include "Piece.hpp"

class Knight : public Piece {
  public:
    Knight(Team t_team, Move::BoardPos t_position, sf::Sprite &t_sprite);
    ~Knight() override = default;

    auto getMoves(std::function<Piece *(Move::BoardPos)> hasPiece, Move::BoardPos board_size) -> MoveList & override;

  private:
    Move::BoardPos start_pos;
};

#endif // KNIGHT_HPP