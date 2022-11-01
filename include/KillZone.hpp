#ifndef KILLZONE_HPP
#define KILLZONE_HPP

#include "Pieces/piece.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class KillZone : sf::RectangleShape {
  public:
    KillZone(sf::Vector2f position, sf::Vector2f size);
    auto draw(sf::RenderWindow &render) -> void;
    auto update(sf::RenderWindow &render) -> void;

    auto addPiece(sf::Sprite &piece) -> void;

  private:
    std::vector<sf::Sprite> pieces_killed;
};

#endif // KILLZONE_HPP