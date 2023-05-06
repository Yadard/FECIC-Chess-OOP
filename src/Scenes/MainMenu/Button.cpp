#include "Scenes/MainMenu/Button.hpp"

Button::Button(std::string t_text) {
    m_background.setFillColor(sf::Color(0, 255, 0, 150));

    m_text.setString(t_text);
    m_text.setFont(AssetManager::GetInstance().getFont("MainMenu.Button"));
    uint32_t font_size = 45;
    m_text.setFillColor(sf::Color::White);
    m_text.setCharacterSize(font_size);
}

auto Button::setBGSize(sf::Vector2f rect_size) -> void { m_background.setSize(rect_size); }

auto Button::setPosition(sf::Vector2f position) -> void {
    m_background.setPosition(position);
    position.x += (float)m_background.getSize().x / 2.0;
    position.y += (float)m_background.getSize().y / 2.0;
    position.x -= (float)m_text.getGlobalBounds().width / 2.0;
    position.y -= m_text.getCharacterSize() / 2.0;
    m_text.setPosition(position);
}

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

auto Button::draw(sf::RenderWindow &render) -> void {
    if (!m_show)
        return;

    render.draw(m_background);
    render.draw(m_text);
}

auto Button::clicked(sf::Event event, sf::FloatRect &mouse) -> bool {
    if (!m_active)
        return false;

    if (event.type != sf::Event::MouseButtonPressed)
        return false;

    if (event.mouseButton.button == sf::Mouse::Left) {
        mouse.left = event.mouseButton.x;
        mouse.top = event.mouseButton.y;
        return mouse.intersects(m_background.getGlobalBounds());
    }

    return false;
}