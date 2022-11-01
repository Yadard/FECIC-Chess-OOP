#include "Pieces/Rook.hpp"

Rook::Rook(Team t_team, Move::BoardPos t_position, sf::Sprite &t_sprite) : Piece(t_team, t_position, t_sprite) {}

auto Rook::getMoves(std::function<Piece *(Move::BoardPos)> hasPiece, Move::BoardPos board_size) -> MoveList & {
    this->_move_list_data.clear();

    // forwards
    Move::BoardPos new_pos = this->goFowards(this->position);
    while (this->inBounds(new_pos, board_size)) {
        Piece *piece = hasPiece(new_pos);
        if (piece) {
            if (isEnemy(piece))
                this->_move_list_data.emplace_back(this, new_pos, this->position);
            break;
        }
        this->_move_list_data.emplace_back(this, new_pos, this->position);
        new_pos = this->goFowards(new_pos);
    }

    // Backwards
    new_pos = this->goBackwards(this->position);
    while (this->inBounds(new_pos, board_size)) {
        Piece *piece = hasPiece(new_pos);
        if (piece) {
            if (isEnemy(piece))
                this->_move_list_data.emplace_back(this, new_pos, this->position);
            break;
        }
        this->_move_list_data.emplace_back(this, new_pos, this->position);
        new_pos = this->goBackwards(new_pos);
    }

    // Left
    new_pos = this->goLeft(this->position);
    while (this->inBounds(new_pos, board_size)) {
        Piece *piece = hasPiece(new_pos);
        if (piece) {
            if (isEnemy(piece))
                this->_move_list_data.emplace_back(this, new_pos, this->position);
            break;
        }
        this->_move_list_data.emplace_back(this, new_pos, this->position);
        new_pos = this->goLeft(new_pos);
    }

    // Right
    new_pos = this->goRight(this->position);
    while (this->inBounds(new_pos, board_size)) {
        Piece *piece = hasPiece(new_pos);
        if (piece) {
            if (isEnemy(piece))
                this->_move_list_data.emplace_back(this, new_pos, this->position);
            break;
        }
        this->_move_list_data.emplace_back(this, new_pos, this->position);
        new_pos = this->goRight(new_pos);
    }

    return this->_move_list_data;
}
