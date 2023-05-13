// clang-format off
#include "pch.hpp"
// clang-format on

#include "Scenes/Chess/Pieces/Knight.hpp"

Knight::Knight(Team t_team, Move::BoardPos t_position, const sf::Texture &t_texture) : Piece(t_team, t_position, t_texture) {}

auto Knight::getMoves(Match *match) -> std::vector<Move> & {
    this->m_move_list_data.clear();

    Move::BoardPos new_pos = this->goRight(this->goForwards(this->position, 2));
    if (this->inBounds(new_pos, match->getBoardSize()) && !isAlly(match->hasPiece(new_pos))) {
        this->m_move_list_data.emplace_back(new_pos, this->position);
    }

    new_pos = this->goRight(this->goForwards(this->position), 2);
    if (this->inBounds(new_pos, match->getBoardSize()) && !isAlly(match->hasPiece(new_pos))) {
        this->m_move_list_data.emplace_back(new_pos, this->position);
    }

    new_pos = this->goLeft(this->goForwards(this->position, 2));
    if (this->inBounds(new_pos, match->getBoardSize()) && !isAlly(match->hasPiece(new_pos))) {
        this->m_move_list_data.emplace_back(new_pos, this->position);
    }

    new_pos = this->goLeft(this->goForwards(this->position), 2);
    if (this->inBounds(new_pos, match->getBoardSize()) && !isAlly(match->hasPiece(new_pos))) {
        this->m_move_list_data.emplace_back(new_pos, this->position);
    }

    new_pos = this->goRight(this->goBackwards(this->position, 2));
    if (this->inBounds(new_pos, match->getBoardSize()) && !isAlly(match->hasPiece(new_pos))) {
        this->m_move_list_data.emplace_back(new_pos, this->position);
    }

    new_pos = this->goRight(this->goBackwards(this->position), 2);
    if (this->inBounds(new_pos, match->getBoardSize()) && !isAlly(match->hasPiece(new_pos))) {
        this->m_move_list_data.emplace_back(new_pos, this->position);
    }

    new_pos = this->goLeft(this->goBackwards(this->position, 2));
    if (this->inBounds(new_pos, match->getBoardSize()) && !isAlly(match->hasPiece(new_pos))) {
        this->m_move_list_data.emplace_back(new_pos, this->position);
    }

    new_pos = this->goLeft(this->goBackwards(this->position), 2);
    if (this->inBounds(new_pos, match->getBoardSize()) && !isAlly(match->hasPiece(new_pos))) {
        this->m_move_list_data.emplace_back(new_pos, this->position);
    }

    return this->m_move_list_data;
}

auto Knight::doSpecialMove(Match *match) -> void {}