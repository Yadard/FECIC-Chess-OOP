#include "KillZone.hpp"

KillZone::KillZone(sf::Vector2f position, sf::Vector2f size) {
    setPosition(position);
    setSize(size);

    setFillColor(sf::Color(80, 80, 80));
}

auto KillZone::draw(sf::RenderWindow &render) -> void {
    float center = (getSize().y - 80) / 2;
    render.draw(*this);
    for (size_t i = 0; i < pieces_killed.size(); i++) {
        pieces_killed[i].setPosition({static_cast<float>(getPosition().x + (i)*50.0 + 10), getPosition().y + center});
        render.draw(pieces_killed[i]);
    }
}

auto KillZone::update(sf::RenderWindow &render) -> void { this->draw(render); }

auto KillZone::addPiece(sf::Sprite &piece) -> void { pieces_killed.emplace_back(piece); }
