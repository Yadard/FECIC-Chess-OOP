// clang-format off
#include "pch.hpp"
// clang-format on

#include "Scenes/Chess/Pieces/Queen.hpp"

Queen::Queen(Team t_team, Move::BoardPos t_position, const sf::Texture &t_texture) : Piece(t_team, t_position, t_texture) {}

auto Queen::getMoves(Match *match) -> std::vector<Move> & {
    this->m_move_list_data.clear();

    // top-right diagonal
    Move::BoardPos new_pos = this->goRight(this->goForwards(this->position));
    while (this->inBounds(new_pos, match->getBoardSize())) {
        Piece *piece = match->hasPiece(new_pos);
        if (piece) {
            if (isEnemy(piece))
                this->m_move_list_data.emplace_back(new_pos, this->position);
            break;
        }
        this->m_move_list_data.emplace_back(new_pos, this->position);
        new_pos = this->goRight(this->goForwards(new_pos));
    }

    // top-left diagonal
    new_pos = this->goLeft(this->goForwards(this->position));
    while (this->inBounds(new_pos, match->getBoardSize())) {
        Piece *piece = match->hasPiece(new_pos);
        if (piece) {
            if (isEnemy(piece))
                this->m_move_list_data.emplace_back(new_pos, this->position);
            break;
        }
        this->m_move_list_data.emplace_back(new_pos, this->position);
        new_pos = this->goLeft(this->goForwards(new_pos));
    }

    // bottom-left diagonal
    new_pos = this->goLeft(this->goBackwards(this->position));
    while (this->inBounds(new_pos, match->getBoardSize())) {
        Piece *piece = match->hasPiece(new_pos);
        if (piece) {
            if (isEnemy(piece))
                this->m_move_list_data.emplace_back(new_pos, this->position);
            break;
        }
        this->m_move_list_data.emplace_back(new_pos, this->position);
        new_pos = this->goLeft(this->goBackwards(new_pos));
    }

    // bottom-right diagonal
    new_pos = this->goRight(this->goBackwards(this->position));
    while (this->inBounds(new_pos, match->getBoardSize())) {
        Piece *piece = match->hasPiece(new_pos);
        if (piece) {
            if (isEnemy(piece))
                this->m_move_list_data.emplace_back(new_pos, this->position);
            break;
        }
        this->m_move_list_data.emplace_back(new_pos, this->position);
        new_pos = this->goRight(this->goBackwards(new_pos));
    }

    new_pos = this->goForwards(this->position);
    while (this->inBounds(new_pos, match->getBoardSize())) {
        Piece *piece = match->hasPiece(new_pos);
        if (piece) {
            if (isEnemy(piece))
                this->m_move_list_data.emplace_back(new_pos, this->position);
            break;
        }
        this->m_move_list_data.emplace_back(new_pos, this->position);
        new_pos = this->goForwards(new_pos);
    }

    // Backwards
    new_pos = this->goBackwards(this->position);
    while (this->inBounds(new_pos, match->getBoardSize())) {
        Piece *piece = match->hasPiece(new_pos);
        if (piece) {
            if (isEnemy(piece))
                this->m_move_list_data.emplace_back(new_pos, this->position);
            break;
        }
        this->m_move_list_data.emplace_back(new_pos, this->position);
        new_pos = this->goBackwards(new_pos);
    }

    // Left
    new_pos = this->goLeft(this->position);
    while (this->inBounds(new_pos, match->getBoardSize())) {
        Piece *piece = match->hasPiece(new_pos);
        if (piece) {
            if (isEnemy(piece))
                this->m_move_list_data.emplace_back(new_pos, this->position);
            break;
        }
        this->m_move_list_data.emplace_back(new_pos, this->position);
        new_pos = this->goLeft(new_pos);
    }

    // Right
    new_pos = this->goRight(this->position);
    while (this->inBounds(new_pos, match->getBoardSize())) {
        Piece *piece = match->hasPiece(new_pos);
        if (piece) {
            if (isEnemy(piece))
                this->m_move_list_data.emplace_back(new_pos, this->position);
            break;
        }
        this->m_move_list_data.emplace_back(new_pos, this->position);
        new_pos = this->goRight(new_pos);
    }

    return m_move_list_data;
}

auto Queen::doSpecialMove(Match *match) -> void {}