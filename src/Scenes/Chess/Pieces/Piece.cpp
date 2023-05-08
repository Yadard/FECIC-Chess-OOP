#include "Scenes/Chess/Pieces/Piece.hpp"

Piece::Piece(Team t_team, Move::BoardPos t_position, const sf::Texture &t_texture) : _team(t_team), position(t_position), onDie(nullptr), onMove(nullptr) {
    this->_move_list_data.reserve(64);
    _sprite.setTexture(t_texture, true);
}

auto Piece::getOldMoves() const -> const MoveList & { return this->_move_list_data; }
auto Piece::getTeam() const -> Team { return _team; }
auto Piece::getSprite() const -> const sf::Sprite & { return _sprite; }
auto Piece::getSprite() -> sf::Sprite & { return _sprite; }

auto Piece::addOldMoves(Move move) -> void { _move_list_data.emplace_back(move); }

auto Piece::goFowards(Move::BoardPos position, size_t amount) -> Move::BoardPos {
    _team == Team::BLACK ? position.y -= amount : position.y += amount;
    return position;
}

auto Piece::goBackwards(Move::BoardPos position, size_t amount) -> Move::BoardPos {
    _team == Team::BLACK ? position.y += amount : position.y -= amount;
    return position;
}

auto Piece::goLeft(Move::BoardPos position, size_t amount) -> Move::BoardPos {
    _team == Team::BLACK ? position.x -= amount : position.x += amount;
    return position;
}

auto Piece::goRight(Move::BoardPos position, size_t amount) -> Move::BoardPos {
    _team == Team::BLACK ? position.x += amount : position.x -= amount;
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