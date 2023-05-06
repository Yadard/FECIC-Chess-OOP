#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "./../../AssetManager.hpp"
#include "./../Scene.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

class Button {
  public:
    Button(std::string t_text);

    auto draw(sf::RenderWindow &render) -> void;
    auto setBGSize(sf::Vector2f rect_size) -> void;
    auto setPosition(sf::Vector2f position) -> void;

    auto hide() -> void;
    auto show() -> void;
    auto deactivate() -> void;
    auto activate() -> void;

    /*
     * @param event: event
     * @return whether or not the event was handled.
     */
    auto clicked(sf::Event event, sf::FloatRect &mouse) -> bool;

  private:
    bool m_active = true;
    bool m_show = true;

    sf::RectangleShape m_background;
    sf::Texture m_texture;
    sf::Text m_text;
};

#endif // BUTTON_HPP
