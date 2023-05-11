#ifndef LOGO_HPP
#define LOGO_HPP

#include "AssetManager.hpp"

class Logo {
  public:
    Logo();

    auto draw(sf::RenderWindow &render) -> void;

    auto setPosition(sf::Vector2f pos) -> void;
    auto getPosition() -> sf::Vector2f;

  private:
    auto updatePosition() -> void;

    sf::Sprite m_img;
    sf::Text m_text;
    sf::Vector2f m_position;
};

#endif // LOGO_HPP