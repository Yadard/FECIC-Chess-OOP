// clang-format off
#include "pch.hpp"
// clang-format on

#include "Scenes/Chess/History.hpp"

History::History(sf::Vector2f position, sf::Vector2f size) {
    setPosition(position);
    setSize(size);
    setFillColor(sf::Color(200, 200, 200));
}

auto History::draw(sf::RenderWindow &render) -> void {
    size_t i = 0;
    render.draw(*this);
    for (auto it = moves.rbegin(); it != moves.rend(); it++, i++) {
        it->sprite.setPosition(this->getPosition().x + 10, this->getPosition().y + i * 60 + 2);
        auto newSize = setSpriteSize(it->sprite, {50.0, 50.0});
        it->text.setPosition(it->sprite.getPosition().x + 60, it->sprite.getPosition().y + (newSize.y - 25) / 2);
        render.draw(it->sprite);
        render.draw(it->text);
    }
}

auto History::update(sf::RenderWindow &render) -> void { this->draw(render); }

auto History::registryMove(Move &move) -> void { moves.emplace_back(move); }

auto History::setSpriteSize(sf::Sprite &sprite, sf::Vector2f size) -> sf::Vector2f {
    float factor = size.x / sprite.getTextureRect().width;
    sprite.setScale(factor, factor);
    return {factor * sprite.getTextureRect().width, factor * sprite.getTextureRect().height};
}