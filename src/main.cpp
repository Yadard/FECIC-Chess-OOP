#include "Game.hpp"

int main() {
    Game game({1024, 960}, "Chess");

    game.loadAssets();

    game.gameloop();

    return EXIT_SUCCESS;
}

/*
#include <SFML/Graphics.hpp>
#include <cmath>
#include <locale>


int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window({640, 480}, "test", sf::Style::Default, settings);

    std::string input_text;
    sf::Font font;
    font.loadFromFile("resources/font.ttf");
    sf::Text text("", font);

    sf::Clock clock;
    Arrow el;
    el.setDimensions({320, 240}, {50, 125}, false);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::TextEntered) {
                if (std::isprint(event.text.unicode))
                    input_text += event.text.unicode;
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::BackSpace) {
                    if (!input_text.empty())
                        input_text.pop_back();
                }
                if (event.key.code == sf::Keyboard::Return) {
                    input_text += '\n';
                }
            }
        }

        static sf::Time text_effect_time;
        static bool show_cursor;

        text_effect_time += clock.restart();

        if (text_effect_time >= sf::seconds(0.5f)) {
            show_cursor = !show_cursor;
            text_effect_time = sf::Time::Zero;
        }

        text.setString(input_text + (show_cursor ? '|' : ' '));

        window.clear();
        window.draw(el);
        window.display();
    }
} */