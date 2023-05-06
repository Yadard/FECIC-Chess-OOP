#include "Scenes/MainMenu/Logo.hpp"

Logo::Logo() {
    m_img.setScale({0.5, 0.5});
    m_img.setTexture(AssetManager::GetInstance().getTexture("MainMenu.Logo"), true);

    m_text.setCharacterSize(125);
    m_text.setFont(AssetManager::GetInstance().getFont("MainMenu.Logo"));
    m_text.setString("Chess-FECIC");

    this->updatePosition();
}

auto Logo::draw(sf::RenderWindow &render) -> void {
    render.draw(m_img);
    render.draw(m_text);
}

auto Logo::updatePosition() -> void {
    m_text.setOrigin({m_text.getLocalBounds().width / 2.0, m_text.getLocalBounds().height / 2.0});
    sf::Vector2f label_pos(m_position.x - m_img.getGlobalBounds().width / 2.0, m_position.y + 50);
    m_text.setPosition(label_pos);
    sf::Vector2f center_img(m_img.getLocalBounds().width / 4.0, m_img.getLocalBounds().height / 4.0);
    m_img.setOrigin(center_img);
    m_img.setPosition({label_pos.x + center_img.x + m_text.getLocalBounds().width / 2.0, m_position.y});
}

auto Logo::setPosition(sf::Vector2f pos) -> void {
    this->m_position = pos;
    this->updatePosition();
}
auto Logo::getPosition() -> sf::Vector2f { return this->m_position; }