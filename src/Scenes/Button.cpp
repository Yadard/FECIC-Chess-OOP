// clang-format off
#include "pch.hpp"
// clang-format on

#include "Scenes/Button.hpp"

auto Button::draw(sf::RenderTarget &target, sf::RenderStates states) const -> void {
    if (!m_show)
        return;

    target.draw(*m_shape);
    target.draw(m_label);
}
auto Button::handleEvent(sf::Event event) -> void {
    if (event.type != sf::Event::MouseButtonPressed)
        return;

    if (event.mouseButton.button != sf::Mouse::Left)
        return;

    sf::Vector2f mouse(event.mouseButton.x, event.mouseButton.y);
    if (m_hitbox.contains(mouse))
        m_action();
}

auto Button::getText() -> sf::Text & { return m_label; }
auto Button::getBackground() -> sf::Drawable * { return m_shape.get(); }
auto Button::setBackground(sf::Drawable *shape) -> void { m_shape.reset(shape); }
auto Button::setAction(Action action) -> void { m_action = action; }
auto Button::setHitbox(sf::FloatRect hitbox) -> void { m_hitbox = hitbox; }

auto Button::hide() -> void {
    m_active = false;
    m_show = false;
}
auto Button::show() -> void {
    m_active = true;
    m_show = true;
}
auto Button::deactivate() -> void { m_active = false; }
auto Button::activate() -> void { m_active = true; }

auto Button::setSize(sf::Vector2f size) -> void {
    m_size = size;
    m_hitbox.width = m_size.x;
    m_hitbox.height = m_size.y;
}
auto Button::getSize() -> sf::Vector2f { return m_size; }