#ifndef HISTORY_HPP
#define HISTORY_HPP

#include "Move.hpp"
#include "Pieces/piece.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

struct Entry {
    Entry(Move t_move) : move(t_move) {
        if (t_move.getPiece())
            sprite = t_move.getPiece()->getSprite();

        if (!loaded) {
            if (!font.loadFromFile("./assets/arial.ttf")) {
                std::cout << "AAA" << std::endl;
            }
            loaded = true;
        }

        text.setFillColor(sf::Color::Black);
        std::string str = "";
        str += static_cast<char>('a' + t_move.getMoveOrigin().x);
        str += static_cast<char>('0' + t_move.getMoveOrigin().y);

        str += "->";

        str += static_cast<char>('a' + t_move.getMoveDestination().x);
        str += static_cast<char>('0' + t_move.getMoveDestination().y);

        text.setString(str.c_str());
        text.setFont(font);
        text.setCharacterSize(25);
    }

    Move move;
    sf::Text text;
    sf::Sprite sprite;
    inline static sf::Font font;
    inline static bool loaded = false;
};

class History : sf::RectangleShape {
  public:
    History(sf::Vector2f position, sf::Vector2f size);
    auto draw(sf::RenderWindow &render) -> void;
    auto update(sf::RenderWindow &render) -> void;

    auto registryMove(Move &move) -> void;

  private:
    auto setSpriteSize(sf::Sprite &sprite, sf::Vector2f size) -> sf::Vector2f;

    std::vector<Entry> moves;
};

#endif // HISTORY_HPP