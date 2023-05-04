#include "Scenes/MainMenu/MainMenu.hpp"

void quit(sf::RenderWindow &render) { render.close(); }

Scene::MainMenu::MainMenu(sf::RenderWindow &render)
    : m_play_btn({60, 660}, "Play"), m_level_selector_btn({380, 660}, "Select Level"), m_quit_btn({710, 660}, "Quit") {
    m_mouse_hitbox.width = 4;
    m_mouse_hitbox.height = 4;

    m_background.setSize({render.getSize().x, render.getSize().y});
    m_background.setPosition({0, 0});
    m_background.setTexture(&AssetManager::GetInstance().getTexture("MainMenu.BG"));
}

auto Scene::MainMenu::update(sf::RenderWindow &render) -> void {
    render.draw(m_background);
    this->m_play_btn.draw(render);
    this->m_level_selector_btn.draw(render);
    this->m_quit_btn.draw(render);
}
auto Scene::MainMenu::draw(sf::RenderWindow &render) -> void {
    render.draw(m_background);
    this->m_play_btn.draw(render);
    this->m_level_selector_btn.draw(render);
    this->m_quit_btn.draw(render);
}

auto Scene::MainMenu::handle_input(std::function<void(Scene::IScene *)> change_scene, std::function<void()> quit, sf::Event event) -> void {
    if (!m_play_btn.hasOnPress())
        m_play_btn.onPress(std::bind([](std::function<void(Scene::IScene *)> change_scene) {}, change_scene));

    if (!m_level_selector_btn.hasOnPress())
        m_level_selector_btn.onPress(std::bind([](std::function<void(Scene::IScene *)> change_scene) {}, change_scene));

    if (!m_quit_btn.hasOnPress())
        m_quit_btn.onPress(std::bind([](std::function<void()> quit) { quit(); }, quit));

    if (m_play_btn.handleMouseEvent(event, m_mouse_hitbox))
        return;
    if (m_level_selector_btn.handleMouseEvent(event, m_mouse_hitbox))
        return;
    if (m_quit_btn.handleMouseEvent(event, m_mouse_hitbox))
        return;
}
