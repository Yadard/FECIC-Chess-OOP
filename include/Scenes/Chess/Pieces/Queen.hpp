#ifndef QUEEN_HPP
#define QUEEN_HPP

#include "Piece.hpp"

class Queen : public Piece {
  public:
    Queen(Team t_team, Move::BoardPos t_position, const sf::Texture &t_texture);

    auto getMoves(Match *match) -> std::vector<Move> & override;
    auto doSpecialMove(Match *match) -> void override;
};

extern "C" PIECE_API_EXPORT Piece *createPiece(Team t_team, Move::BoardPos t_position, const sf::Texture &t_texture) {
    return new Queen(t_team, t_position, t_texture);
}

#endif // QUEEN_HPP