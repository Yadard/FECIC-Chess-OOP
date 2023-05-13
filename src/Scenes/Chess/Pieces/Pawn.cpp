// clang-format off
#include "pch.hpp"
// clang-format on

#include "Scenes/Chess/Pieces/Pawn.hpp"

Pawn::Pawn(Team t_team, Move::BoardPos t_position, const sf::Texture &t_texture) : Piece(t_team, t_position, t_texture), m_start_pos(t_position) {
    this->onReachEnd = [&](Match *match, std::unique_ptr<Piece> &piece) {
        Piece *temp = nullptr;
        if (piece->getTeam() == Team::WHITE) {
            temp = match->createPiece("Queen", Team::WHITE, piece->position);
        } else {
            temp = match->createPiece("Queen", Team::BLACK, piece->position);
        }
        for (auto move : m_move_list_data) {
            temp->addOldMoves(move);
        }

        temp->getSprite().setPosition(piece->getSprite().getPosition());
        temp->getSprite().setScale(piece->getSprite().getScale());

        piece.reset(temp);
    };
}

auto Pawn::getMoves(Match *match) -> std::vector<Move> & {
    this->m_move_list_data.clear();

    Move::BoardPos pos = goLeft(goForwards(position));
    if (inBounds(pos, match->getBoardSize()) && isEnemy(match->hasPiece(pos)))
        m_move_list_data.emplace_back(pos, this->position);

    pos = goRight(goForwards(position));
    if (inBounds(pos, match->getBoardSize()) && isEnemy(match->hasPiece(pos)))
        m_move_list_data.emplace_back(pos, this->position);

    pos = goForwards(position);
    if (inBounds(pos, match->getBoardSize()) && !match->hasPiece(pos))
        m_move_list_data.emplace_back(pos, this->position);

    pos = goForwards(position, 2);
    if (m_start_pos == this->position && inBounds(pos, match->getBoardSize()) && !match->hasPiece(pos)) {
        m_move_list_data.emplace_back(pos, this->position);
    }

    return this->m_move_list_data;
}

auto Pawn::doSpecialMove(Match *match) -> void {
    Move::BoardPos pos = this->m_start_pos;
    if (!match->hasPiece(pos) && m_ability_available) {
        m_ability_available = false;
        match->addPiece(match->createPiece("Queen", this->m_team, pos), pos);
        match->endTurn();
    }
}