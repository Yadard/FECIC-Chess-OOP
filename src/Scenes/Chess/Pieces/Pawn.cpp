// clang-format off
#include "pch.hpp"
// clang-format on

#include "Scenes/Chess/Pieces/Pawn.hpp"

Pawn::Pawn(Team t_team, Move::BoardPos t_position, const sf::Texture &t_texture) : Piece(t_team, t_position, t_texture), start_pos(t_position) {
    this->onReachEnd = [&](Match *match, std::unique_ptr<Piece> &piece) {
        Piece *temp = nullptr;
        if (piece->getTeam() == Team::WHITE) {
            temp = match->createPiece("Queen", Team::WHITE, piece->position);
        } else {
            temp = match->createPiece("Queen", Team::BLACK, piece->position);
        }
        for (auto move : _move_list_data) {
            temp->addOldMoves(move);
        }

        temp->getSprite().setPosition(piece->getSprite().getPosition());
        temp->getSprite().setScale(piece->getSprite().getScale());

        piece.reset(temp);
    };
}

auto Pawn::getMoves(std::function<Piece *(Move::BoardPos)> hasPiece, Move::BoardPos board_size) -> MoveList & {
    this->_move_list_data.clear();

    Move::BoardPos pos = goLeft(goFowards(position));
    if (inBounds(pos, board_size) && isEnemy(hasPiece(pos)))
        _move_list_data.emplace_back(this, pos, this->position);

    pos = goRight(goFowards(position));
    if (inBounds(pos, board_size) && isEnemy(hasPiece(pos)))
        _move_list_data.emplace_back(this, pos, this->position);

    pos = goFowards(position);
    if (inBounds(pos, board_size) && !hasPiece(pos))
        _move_list_data.emplace_back(this, pos, this->position);

    pos = goFowards(position, 2);
    if (start_pos == this->position && inBounds(pos, board_size) && !hasPiece(pos)) {
        _move_list_data.emplace_back(this, pos, this->position);
    }

    return this->_move_list_data;
}
