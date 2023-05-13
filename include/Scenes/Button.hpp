#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>

class Button : public sf::Drawable, public sf::Transformable {
  public:
    using Action = std::function<bool()>;

    auto draw(sf::RenderTarget &target, sf::RenderStates states) const -> void override;
    auto handleEvent(sf::Event event) -> bool;

    auto getText() -> sf::Text &;
    auto getBackground() -> sf::Drawable *;
    auto setBackground(sf::Drawable *bg) -> void;
    auto setAction(Action action) -> void;

    auto hide() -> void;
    auto show() -> void;
    auto deactivate() -> void;
    auto activate() -> void;
    auto setHitbox(sf::FloatRect hitbox) -> void;

    auto setSize(sf::Vector2f size) -> void;
    auto getSize() -> sf::Vector2f;

  private:
    bool m_show = true;
    bool m_active = true;

    std::unique_ptr<sf::Drawable> m_shape;
    sf::FloatRect m_hitbox;
    sf::Vector2f m_size;
    sf::Text m_label;
    Action m_action;
};

#endif // BUTTON_HPP