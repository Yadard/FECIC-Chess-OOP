#ifndef VICTORY_HPP
#define VICTORY_HPP

#include "./../AssetManager.hpp"
#include "./MainMenu/MainMenu.hpp"
#include "./Scene.hpp"
#include "Button.hpp"
#include <SFML/Graphics.hpp>

class Victory : public sf::Drawable {
  public:
    Victory(sf::RenderWindow &render, std::function<void()> t_quit, std::function<void(Scene::IScene *)> t_change_scene, Team winner, Preset preset,
            bool new_board);

    auto draw(sf::RenderTarget &target, sf::RenderStates states) const -> void override;
    auto handleInput(sf::Event) -> void;

  private:
    auto setupButton(Button &btn, size_t index, sf::Vector2f size, sf::Vector2f pos) -> void;

    struct {
        Button m_play_again;
        Button m_save_replay;
        Button m_save_board;
        Button m_mainmenu;
    } btns;

    sf::Text m_label;
    sf::RectangleShape m_bg;
};

#endif // VICTORY_HPP