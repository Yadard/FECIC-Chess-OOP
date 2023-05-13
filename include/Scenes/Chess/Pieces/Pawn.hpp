#ifndef PAWN_HPP
#define PAWN_HPP

#include "Piece.hpp"

class Pawn : public Piece {
  public:
    Pawn(Team t_team, Move::BoardPos t_position, const sf::Texture &t_texture);

    auto getMoves(Match *match) -> std::vector<Move> & override;
    auto doSpecialMove(Match *match) -> void override;

  private:
    Move::BoardPos m_start_pos;
    bool m_ability_available = true;
};

extern "C" PIECE_API_EXPORT Piece *createPiece(Team t_team, Move::BoardPos t_position, const sf::Texture &t_texture) {
    return new Pawn(t_team, t_position, t_texture);
}

#endif // PAWN_HPP