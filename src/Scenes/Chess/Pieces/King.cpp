// clang-format off
#include "pch.hpp"
// clang-format on

#include "Scenes/Chess/Pieces/King.hpp"

King::King(Team t_team, Move::BoardPos t_position, const sf::Texture &t_texture) : Piece(t_team, t_position, t_texture) {
    onDie = [&](Match *match) { match->win(t_team); };
}

auto King::getMoves(Match *match) -> std::vector<Move> & {
    this->m_move_list_data.clear();

    Move::BoardPos new_pos = this->goForwards(this->position);
    if (this->inBounds(new_pos, match->getBoardSize()) && !this->isAlly(match->hasPiece(new_pos)))
        this->m_move_list_data.emplace_back(new_pos, this->position);

    new_pos = this->goLeft(this->goForwards(this->position));
    if (this->inBounds(new_pos, match->getBoardSize()) && !this->isAlly(match->hasPiece(new_pos)))
        this->m_move_list_data.emplace_back(new_pos, this->position);

    new_pos = this->goRight(this->goForwards(this->position));
    if (this->inBounds(new_pos, match->getBoardSize()) && !this->isAlly(match->hasPiece(new_pos)))
        this->m_move_list_data.emplace_back(new_pos, this->position);

    new_pos = this->goRight(this->position);
    if (this->inBounds(new_pos, match->getBoardSize()) && !this->isAlly(match->hasPiece(new_pos)))
        this->m_move_list_data.emplace_back(new_pos, this->position);

    new_pos = this->goLeft(this->position);
    if (this->inBounds(new_pos, match->getBoardSize()) && !this->isAlly(match->hasPiece(new_pos)))
        this->m_move_list_data.emplace_back(new_pos, this->position);

    new_pos = this->goBackwards(this->position);
    if (this->inBounds(new_pos, match->getBoardSize()) && !this->isAlly(match->hasPiece(new_pos)))
        this->m_move_list_data.emplace_back(new_pos, this->position);

    new_pos = this->goLeft(this->goBackwards(this->position));
    if (this->inBounds(new_pos, match->getBoardSize()) && !this->isAlly(match->hasPiece(new_pos)))
        this->m_move_list_data.emplace_back(new_pos, this->position);

    new_pos = this->goRight(this->goBackwards(this->position));
    if (this->inBounds(new_pos, match->getBoardSize()) && !this->isAlly(match->hasPiece(new_pos)))
        this->m_move_list_data.emplace_back(new_pos, this->position);

    return m_move_list_data;
}

auto King::doSpecialMove(Match *match) -> void {}