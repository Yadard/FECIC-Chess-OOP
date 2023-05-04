#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "./../../AssetManager.hpp"
#include "./../Scene.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

class Button {
  public:
    Button(sf::Vector2f position, std::string t_text);

    auto press() -> void;

    auto draw(sf::RenderWindow &render) -> void;

    /*
     * @param event: event
     * @return whether or not the event was handled.
     */
    auto handleMouseEvent(sf::Event event, sf::FloatRect &mouse) -> bool;

    auto onPress(std::function<void()> onPress) -> void;
    auto onRelease(std::function<void()> onRelease) -> void;
    auto hasOnPress() -> bool;
    auto hasOnRelease() -> bool;

  private:
    struct {
        std::function<void()> onPress = nullptr;
        std::function<void()> onRelease = nullptr;
    } m_callbacks;
    sf::RectangleShape m_background;
    sf::Texture m_texture;
    sf::Text m_text;
};

#endif // BUTTON_HPP
