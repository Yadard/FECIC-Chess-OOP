#ifndef KNIGHT_HPP
#define KNIGHT_HPP

#include "Piece.hpp"

class Knight : public Piece {
  public:
    Knight(Team t_team, Move::BoardPos t_position, const sf::Texture &t_texture);

    auto getMoves(std::function<Piece *(Move::BoardPos)> hasPiece, Move::BoardPos board_size) -> MoveList & override;

  private:
    Move::BoardPos start_pos;
};

extern "C" PIECE_API_EXPORT Piece *createPiece(Team t_team, Move::BoardPos t_position, const sf::Texture &t_texture) {
    return new Knight(t_team, t_position, t_texture);
}

#endif // KNIGHT_HPP