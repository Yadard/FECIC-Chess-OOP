// clang-format off
#include "pch.hpp"
// clang-format on

#include "Scenes/Chess/Move.hpp"

Move::Move(BoardPos t_destination, BoardPos t_origin) : m_destination(t_destination), m_origin(t_origin) {}
auto Move::getMoveDestination() const -> BoardPos { return m_destination; }
auto Move::getMoveOrigin() const -> BoardPos { return m_origin; }