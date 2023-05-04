#include "Pieces/King.hpp"

King::King(Team t_team, Move::BoardPos t_position, const sf::Sprite &t_sprite, std::function<void()> t_ondie) : Piece(t_team, t_position, t_sprite) {
    this->onDie = t_ondie;
}

auto King::getMoves(std::function<Piece *(Move::BoardPos)> hasPiece, Move::BoardPos board_size) -> MoveList & {
    this->_move_list_data.clear();

    Move::BoardPos new_pos = this->goFowards(this->position);
    if (this->inBounds(new_pos, board_size) && !this->isAlly(hasPiece(new_pos)))
        this->_move_list_data.emplace_back(this, new_pos, this->position);

    new_pos = this->goLeft(this->goFowards(this->position));
    if (this->inBounds(new_pos, board_size) && !this->isAlly(hasPiece(new_pos)))
        this->_move_list_data.emplace_back(this, new_pos, this->position);

    new_pos = this->goRight(this->goFowards(this->position));
    if (this->inBounds(new_pos, board_size) && !this->isAlly(hasPiece(new_pos)))
        this->_move_list_data.emplace_back(this, new_pos, this->position);

    new_pos = this->goRight(this->position);
    if (this->inBounds(new_pos, board_size) && !this->isAlly(hasPiece(new_pos)))
        this->_move_list_data.emplace_back(this, new_pos, this->position);

    new_pos = this->goLeft(this->position);
    if (this->inBounds(new_pos, board_size) && !this->isAlly(hasPiece(new_pos)))
        this->_move_list_data.emplace_back(this, new_pos, this->position);

    new_pos = this->goBackwards(this->position);
    if (this->inBounds(new_pos, board_size) && !this->isAlly(hasPiece(new_pos)))
        this->_move_list_data.emplace_back(this, new_pos, this->position);

    new_pos = this->goLeft(this->goBackwards(this->position));
    if (this->inBounds(new_pos, board_size) && !this->isAlly(hasPiece(new_pos)))
        this->_move_list_data.emplace_back(this, new_pos, this->position);

    new_pos = this->goRight(this->goBackwards(this->position));
    if (this->inBounds(new_pos, board_size) && !this->isAlly(hasPiece(new_pos)))
        this->_move_list_data.emplace_back(this, new_pos, this->position);

    return _move_list_data;
}
