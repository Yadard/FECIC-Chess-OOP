#ifndef KING_HPP
#define KING_HPP

#include "Piece.hpp"

class King : public Piece {
  public:
    King(Team t_team, Move::BoardPos t_position, const sf::Texture &t_texture);

    auto getMoves(std::function<Piece *(Move::BoardPos)> hasPiece, Move::BoardPos board_size) -> MoveList & override;
};

extern "C" PIECE_API_EXPORT Piece *createPiece(Team t_team, Move::BoardPos t_position, const sf::Texture &t_texture) {
    return new King(t_team, t_position, t_texture);
}

#endif // KING_HPP