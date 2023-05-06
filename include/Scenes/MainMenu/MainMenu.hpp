#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include "./../../AssetManager.hpp"
#include "./../Scene.hpp"
#include "./Popup/Play.hpp"
#include "Button.hpp"
#include "Logo.hpp"
#include <iostream>

namespace Scene {

class MainMenu : public IScene {
  public:
    MainMenu(sf::RenderWindow &render);

    auto update(sf::RenderWindow &render) -> void override;
    auto draw(sf::RenderWindow &render) -> void override;
    auto handle_input(std::function<void(Scene::IScene *)> change_scene, std::function<void()> quit, sf::Event event) -> void override;

  private:
    Button m_play_btn;
    Button m_replay_btn;
    Button m_quit_btn;
    Logo m_logo;

    sf::Vector2f m_piece_origin_pos;
    sf::Vector2f m_tile_size;
    sf::RectangleShape m_background;
    sf::RectangleShape m_selected_highlight;
    sf::Sprite m_piece;
    sf::FloatRect m_mouse_hitbox;

    PlayPopup m_popup;

    sf::Sound m_move_sfx;
};
} // namespace Scene

#endif // MAINMENU_HPP