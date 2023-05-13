#ifndef BISHOP_HPP
#define BISHOP_HPP

#include "Piece.hpp"

class Bishop : public Piece {
  public:
    Bishop(Team t_team, Move::BoardPos t_position, const sf::Texture &t_texture);

    auto getMoves(Match *match) -> std::vector<Move> & override;
    auto doSpecialMove(Match *match) -> void override;
};

extern "C" PIECE_API_EXPORT Piece *createPiece(Team t_team, Move::BoardPos t_position, const sf::Texture &t_texture) {
    return new Bishop(t_team, t_position, t_texture);
}

#endif // BISHOP_HPP