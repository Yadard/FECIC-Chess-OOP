#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include "./../../AssetManager.hpp"
#include "./../Scene.hpp"
#include "Button.hpp"


namespace Scene {

class MainMenu : public IScene {
  public:
    MainMenu(sf::RenderWindow &render);

    auto update(sf::RenderWindow &render) -> void override;
    auto draw(sf::RenderWindow &render) -> void override;
    auto handle_input(std::function<void(Scene::IScene *)> change_scene, std::function<void()> quit, sf::Event event) -> void override;

  private:
    Button m_play_btn;
    Button m_level_selector_btn;
    Button m_quit_btn;
    sf::RectangleShape m_background;
    sf::FloatRect m_mouse_hitbox;
};
} // namespace Scene

#endif // MAINMENU_HPP