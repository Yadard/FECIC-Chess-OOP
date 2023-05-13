// clang-format off
#include "pch.hpp"
// clang-format on

#include "Scenes/Chess/History.hpp"

History::History(sf::Vector2f position, sf::Vector2f size) {
    m_bg.setPosition(position);
    m_bg.setSize(size);
    m_bg.setFillColor(sf::Color(200, 200, 200));
}

auto History::draw(sf::RenderTarget &target, sf::RenderStates states) const -> void {
    target.draw(m_bg);
    for (auto it = m_moves.rbegin(); it != m_moves.rend(); it++) {
        target.draw(it->sprite);
        target.draw(it->text);
    }
}
auto History::registryMove(const Move &move, sf::Sprite &sprite) -> void {
    m_moves.emplace_back(move, sprite);
    Entry &entry = m_moves.back();

    entry.sprite.setPosition(m_bg.getPosition().x + 10, m_bg.getPosition().y + (m_moves.size() - 1) * 60 + 2);
    auto newSize = setSpriteSize(entry.sprite, {50.0, 50.0});
    entry.text.setPosition(entry.sprite.getPosition().x + 60, entry.sprite.getPosition().y + (newSize.y - 25) / 2);
}

auto History::removeLastMove() -> void { m_moves.pop_back(); }

auto History::setSpriteSize(sf::Sprite &sprite, sf::Vector2f size) -> sf::Vector2f {
    float factor = size.x / sprite.getTextureRect().width;
    sprite.setScale(factor, factor);
    return {factor * sprite.getTextureRect().width, factor * sprite.getTextureRect().height};
}