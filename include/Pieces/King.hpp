#ifndef KING_HPP
#define KING_HPP

#include "Piece.hpp"

class King : public Piece {
  public:
    King(Team t_team, Move::BoardPos t_position, sf::Sprite &t_sprite, std::function<void()> t_ondie);
    ~King() override = default;

    auto getMoves(std::function<Piece *(Move::BoardPos)> hasPiece, Move::BoardPos board_size) -> MoveList & override;
};

#endif // KING_HPP