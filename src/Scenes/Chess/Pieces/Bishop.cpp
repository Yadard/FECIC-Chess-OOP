#include "Scenes/Chess/Pieces/Bishop.hpp"

Bishop::Bishop(Team t_team, Move::BoardPos t_position, const sf::Texture &t_texture) : Piece(t_team, t_position, t_texture) {}

auto Bishop::getMoves(std::function<Piece *(Move::BoardPos)> hasPiece, Move::BoardPos board_size) -> MoveList & {
    this->_move_list_data.clear();

    // top-right diagonal
    Move::BoardPos new_pos = this->goRight(this->goFowards(this->position));
    while (this->inBounds(new_pos, board_size)) {
        Piece *piece = hasPiece(new_pos);
        if (piece) {
            if (isEnemy(piece))
                this->_move_list_data.emplace_back(this, new_pos, this->position);
            break;
        }
        this->_move_list_data.emplace_back(this, new_pos, this->position);
        new_pos = this->goRight(this->goFowards(new_pos));
    }

    // top-left diagonal
    new_pos = this->goLeft(this->goFowards(this->position));
    while (this->inBounds(new_pos, board_size)) {
        Piece *piece = hasPiece(new_pos);
        if (piece) {
            if (isEnemy(piece))
                this->_move_list_data.emplace_back(this, new_pos, this->position);
            break;
        }
        this->_move_list_data.emplace_back(this, new_pos, this->position);
        new_pos = this->goLeft(this->goFowards(new_pos));
    }

    // bottom-left diagonal
    new_pos = this->goLeft(this->goBackwards(this->position));
    while (this->inBounds(new_pos, board_size)) {
        Piece *piece = hasPiece(new_pos);
        if (piece) {
            if (isEnemy(piece))
                this->_move_list_data.emplace_back(this, new_pos, this->position);
            break;
        }
        this->_move_list_data.emplace_back(this, new_pos, this->position);
        new_pos = this->goLeft(this->goBackwards(new_pos));
    }

    // bottom-right diagonal
    new_pos = this->goRight(this->goBackwards(this->position));
    while (this->inBounds(new_pos, board_size)) {
        Piece *piece = hasPiece(new_pos);
        if (piece) {
            if (isEnemy(piece))
                this->_move_list_data.emplace_back(this, new_pos, this->position);
            break;
        }
        this->_move_list_data.emplace_back(this, new_pos, this->position);
        new_pos = this->goRight(this->goBackwards(new_pos));
    }

    return _move_list_data;
}
