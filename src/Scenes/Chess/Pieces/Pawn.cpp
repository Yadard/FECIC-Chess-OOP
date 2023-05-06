#include "Scenes/Chess/Pieces/Pawn.hpp"

Pawn::Pawn(Team t_team, Move::BoardPos t_position, const sf::Sprite &t_sprite) : Piece(t_team, t_position, t_sprite), start_pos(t_position) {
    this->onReachEnd = [](std::unique_ptr<Piece> &piece, std::unordered_map<std::string, sf::Sprite> &sprites) {
        if (piece->getTeam() == Team::WHITE)
            piece.reset(new Queen(piece->getTeam(), piece->position, sprites["Starter_queen"]));
        else
            piece.reset(new Queen(piece->getTeam(), piece->position, sprites["Latter_queen"]));
    };
}

auto Pawn::getMoves(std::function<Piece *(Move::BoardPos)> hasPiece, Move::BoardPos board_size) -> MoveList & {
    this->_move_list_data.clear();

    Move::BoardPos pos = goLeft(goFowards(position));
    if (isEnemy(hasPiece(pos)) && inBounds(pos, board_size))
        _move_list_data.emplace_back(this, pos, this->position);

    pos = goRight(goFowards(position));
    if (isEnemy(hasPiece(pos)) && inBounds(pos, board_size))
        _move_list_data.emplace_back(this, pos, this->position);

    pos = goFowards(position);
    if (!hasPiece(pos) && inBounds(pos, board_size))
        _move_list_data.emplace_back(this, pos, this->position);

    pos = goFowards(position, 2);
    if (start_pos == this->position && inBounds(pos, board_size) && !hasPiece(pos)) {
        _move_list_data.emplace_back(this, pos, this->position);
    }

    return this->_move_list_data;
}
