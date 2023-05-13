#ifndef HISTORY_HPP
#define HISTORY_HPP

#include "AssetManager.hpp"
#include "Pieces/piece.hpp"
#include "Scenes/Chess/Move.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

struct Entry {
    Entry(Move t_move, sf::Sprite &t_sprite) : move(t_move), sprite(t_sprite) {

        text.setFillColor(sf::Color::Black);
        std::string str = "";
        str += static_cast<char>('a' + t_move.getMoveOrigin().x);
        str += static_cast<char>('0' + t_move.getMoveOrigin().y);

        str += "->";

        str += static_cast<char>('a' + t_move.getMoveDestination().x);
        str += static_cast<char>('0' + t_move.getMoveDestination().y);

        text.setString(str.c_str());
        text.setFont(AssetManager::GetInstance().getFont("MainMenu.Button"));
        text.setCharacterSize(25);
    }

    Move move;
    sf::Text text;
    sf::Sprite sprite;
    inline static sf::Font font;
    inline static bool loaded = false;
};

class History : public sf::Drawable {
  public:
    History(sf::Vector2f position, sf::Vector2f size);
    auto draw(sf::RenderTarget &target, sf::RenderStates states) const -> void override;

    auto registryMove(const Move &move, sf::Sprite &sprite) -> void;
    auto removeLastMove() -> void;

  private:
    auto setSpriteSize(sf::Sprite &sprite, sf::Vector2f size) -> sf::Vector2f;

    bool m_need_update = true;
    std::vector<Entry> m_moves;
    sf::RectangleShape m_bg;
};

#endif // HISTORY_HPP