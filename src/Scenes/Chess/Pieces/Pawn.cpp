#include "Scenes/Chess/Pieces/Pawn.hpp"

Pawn::Pawn(Team t_team, Move::BoardPos t_position, const sf::Texture &t_texture) : Piece(t_team, t_position, t_texture), start_pos(t_position) {
    this->onReachEnd = [](std::unique_ptr<Piece> &piece, std::unordered_map<std::string, sf::Sprite> &sprites) {
        if (piece->getTeam() == Team::WHITE) {
            piece.reset(AssetManager::GetInstance().piece_factory.createPiece("Queen_White", Team::WHITE, piece->position));
        } else {
            piece.reset(AssetManager::GetInstance().piece_factory.createPiece("Queen_Black", Team::BLACK, piece->position));
        }
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
