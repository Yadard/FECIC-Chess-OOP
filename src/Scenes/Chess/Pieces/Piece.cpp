// clang-format off
#include "pch.hpp"
// clang-format on

#include "Scenes/Chess/Pieces/Piece.hpp"

Piece::Piece(Team t_team, Move::BoardPos t_position, const sf::Texture &t_texture) : m_team(t_team), position(t_position), onDie(nullptr), onMove(nullptr) {
    this->m_move_list_data.reserve(100);
    m_sprite.setTexture(t_texture, true);
}

auto Piece::getOldMoves() const -> const std::vector<Move> & { return this->m_move_list_data; }
auto Piece::getTeam() const -> Team { return m_team; }
auto Piece::getSprite() const -> const sf::Sprite & { return m_sprite; }
auto Piece::getSprite() -> sf::Sprite & { return m_sprite; }

auto Piece::addOldMoves(Move move) -> void { m_move_list_data.emplace_back(move); }

auto Piece::goForwards(Move::BoardPos position, size_t amount) -> Move::BoardPos {
    m_team == Team::BLACK ? position.y -= amount : position.y += amount;
    return position;
}

auto Piece::goBackwards(Move::BoardPos position, size_t amount) -> Move::BoardPos {
    m_team == Team::BLACK ? position.y += amount : position.y -= amount;
    return position;
}

auto Piece::goLeft(Move::BoardPos position, size_t amount) -> Move::BoardPos {
    m_team == Team::BLACK ? position.x -= amount : position.x += amount;
    return position;
}

auto Piece::goRight(Move::BoardPos position, size_t amount) -> Move::BoardPos {
    m_team == Team::BLACK ? position.x += amount : position.x -= amount;
    return position;
}

auto Piece::inBounds(Move::BoardPos position, Move::BoardPos board_size) -> bool {
    return (position.x >= 0 && position.x < board_size.x) && (position.y >= 0 && position.y < board_size.x);
}

auto Piece::isEnemy(Piece *piece) -> bool {
    if (!piece)
        return false;
    return piece->getTeam() != this->m_team;
}
auto Piece::isAlly(Piece *piece) -> bool {
    if (!piece)
        return false;
    return piece->getTeam() == this->m_team;
}
