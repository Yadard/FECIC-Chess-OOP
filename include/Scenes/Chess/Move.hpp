#ifndef MOVE_HPP
#define MOVE_HPP

#include <SFML/Graphics.hpp>

class Move {
  public:
    using BoardPos = sf::Vector2u;
    Move(BoardPos t_destination, BoardPos t_origin);

    auto getMoveDestination() const -> BoardPos;
    auto getMoveOrigin() const -> BoardPos;

  private:
    BoardPos m_destination;
    BoardPos m_origin;
};

#endif // MOVE_HPP