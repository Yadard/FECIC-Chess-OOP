#include "Scenes/MainMenu/Button.hpp"

Button::Button(sf::Vector2f position, std::string t_text) {
    m_texture = AssetManager::GetInstance().getTexture("MainMenu.ButtonBG");
    m_texture.setRepeated(true);

    sf::Vector2f rect_size(300, 280);
    m_background.setSize(rect_size);
    m_background.setTexture(&m_texture, true);
    m_background.setPosition(position);

    m_text.setString(t_text);
    m_text.setFont(AssetManager::GetInstance().getFont("MainMenu.Button"));
    uint32_t font_size = 45;
    m_text.setFillColor(sf::Color::White);
    m_text.setCharacterSize(font_size);
    sf::Vector2f pos(position.x, position.y);
    pos.x += (float)rect_size.x / 2.0;
    pos.y += (float)rect_size.y / 2.0;
    pos.x -= (float)m_text.getGlobalBounds().width / 2.0;
    pos.y -= font_size / 2.0;
    m_text.setPosition(pos);
}

auto Button::press() -> void {
    if (m_callbacks.onPress)
        m_callbacks.onPress();
}

auto Button::draw(sf::RenderWindow &render) -> void {
    render.draw(m_background);
    render.draw(m_text);
}

auto Button::handleMouseEvent(sf::Event event, sf::FloatRect &mouse) -> bool {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            mouse.left = event.mouseButton.x;
            mouse.top = event.mouseButton.y;
            if (mouse.intersects(m_background.getGlobalBounds()) && m_callbacks.onPress) {
                m_callbacks.onPress();
                return true;
            }
        }
    }
    return false;
}

auto Button::onPress(std::function<void()> onPress) -> void { m_callbacks.onPress = onPress; }
auto Button::onRelease(std::function<void()> onRelease) -> void { m_callbacks.onRelease = onRelease; }
auto Button::hasOnPress() -> bool { return m_callbacks.onPress != nullptr; }
auto Button::hasOnRelease() -> bool { return m_callbacks.onRelease != nullptr; }