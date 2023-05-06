#include "Scenes/Chess/Pieces/Knight.hpp"

Knight::Knight(Team t_team, Move::BoardPos t_position, const sf::Texture &t_texture) : Piece(t_team, t_position, t_texture) {}

auto Knight::getMoves(std::function<Piece *(Move::BoardPos)> hasPiece, Move::BoardPos board_size) -> MoveList & {
    this->_move_list_data.clear();

    Move::BoardPos new_pos = this->goRight(this->goFowards(this->position, 2));
    if (this->inBounds(new_pos, board_size) && !isAlly(hasPiece(new_pos))) {
        this->_move_list_data.emplace_back(this, new_pos, this->position);
    }

    new_pos = this->goRight(this->goFowards(this->position), 2);
    if (this->inBounds(new_pos, board_size) && !isAlly(hasPiece(new_pos))) {
        this->_move_list_data.emplace_back(this, new_pos, this->position);
    }

    new_pos = this->goLeft(this->goFowards(this->position, 2));
    if (this->inBounds(new_pos, board_size) && !isAlly(hasPiece(new_pos))) {
        this->_move_list_data.emplace_back(this, new_pos, this->position);
    }

    new_pos = this->goLeft(this->goFowards(this->position), 2);
    if (this->inBounds(new_pos, board_size) && !isAlly(hasPiece(new_pos))) {
        this->_move_list_data.emplace_back(this, new_pos, this->position);
    }

    new_pos = this->goRight(this->goBackwards(this->position, 2));
    if (this->inBounds(new_pos, board_size) && !isAlly(hasPiece(new_pos))) {
        this->_move_list_data.emplace_back(this, new_pos, this->position);
    }

    new_pos = this->goRight(this->goBackwards(this->position), 2);
    if (this->inBounds(new_pos, board_size) && !isAlly(hasPiece(new_pos))) {
        this->_move_list_data.emplace_back(this, new_pos, this->position);
    }

    new_pos = this->goLeft(this->goBackwards(this->position, 2));
    if (this->inBounds(new_pos, board_size) && !isAlly(hasPiece(new_pos))) {
        this->_move_list_data.emplace_back(this, new_pos, this->position);
    }

    new_pos = this->goLeft(this->goBackwards(this->position), 2);
    if (this->inBounds(new_pos, board_size) && !isAlly(hasPiece(new_pos))) {
        this->_move_list_data.emplace_back(this, new_pos, this->position);
    }

    return this->_move_list_data;
}
