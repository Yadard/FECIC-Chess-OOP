#include "Scenes/Chess/Pieces/Piece.hpp"

Piece::Piece(Team t_team, Move::BoardPos t_position, const sf::Sprite &t_sprite)
    : _team(t_team), position(t_position), _sprite(t_sprite), onDie(nullptr), onMove(nullptr) {
    this->_move_list_data.reserve(64);
}

auto Piece::getOldMoves() -> const MoveList & { return this->_move_list_data; }
auto Piece::getTeam() -> Team { return _team; }
auto Piece::getSprite() -> sf::Sprite & { return _sprite; }

auto Piece::goFowards(Move::BoardPos position, size_t amount) -> Move::BoardPos {
    _team == Team::LATTER ? position.y -= amount : position.y += amount;
    return position;
}

auto Piece::goBackwards(Move::BoardPos position, size_t amount) -> Move::BoardPos {
    _team == Team::LATTER ? position.y += amount : position.y -= amount;
    return position;
}

auto Piece::goLeft(Move::BoardPos position, size_t amount) -> Move::BoardPos {
    _team == Team::LATTER ? position.x -= amount : position.x += amount;
    return position;
}

auto Piece::goRight(Move::BoardPos position, size_t amount) -> Move::BoardPos {
    _team == Team::LATTER ? position.x += amount : position.x -= amount;
    return position;
}

auto Piece::inBounds(Move::BoardPos position, Move::BoardPos board_size) -> bool {
    return (position.x >= 0 && position.x < board_size.x) && (position.y >= 0 && position.y < board_size.x);
}

auto Piece::isEnemy(Piece *piece) -> bool {
    if (!piece)
        return false;
    return piece->getTeam() != this->_team;
}
auto Piece::isAlly(Piece *piece) -> bool {
    if (!piece)
        return false;
    return piece->getTeam() == this->_team;
}