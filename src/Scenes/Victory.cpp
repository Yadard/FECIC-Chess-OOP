// clang-format off
#include "pch.hpp"
// clang-format on

#include "Scenes/Victory.hpp"

Victory::Victory(sf::RenderWindow &render, std::function<void()> t_quit, std::function<void(Scene::IScene *)> t_change_scene, Team winner, Preset preset,
                 bool new_board) {
    sf::Vector2f viewport(render.getSize().x, render.getSize().y);
    sf::Vector2f pos(viewport.x * 0.5f, viewport.y * 0.4f);
    sf::Vector2f size(viewport.x * 0.6f, viewport.y * 0.4f);
    m_bg.setSize(size);
    size.x *= 0.5f;
    size.y *= 0.5f;
    m_bg.setOrigin(size);
    m_bg.setPosition(pos);
    m_bg.setFillColor(sf::Color(100, 100, 100, 150));

    m_label.setFont(AssetManager::GetInstance().getFont("MainMenu.Button"));
    m_label.setFillColor(sf::Color::White);
    if (winner == Team::WHITE)
        m_label.setString("White team won!!!");
    else
        m_label.setString("Black team won!!!");

    m_label.setCharacterSize(80);
    m_label.setPosition({0, 0});
    m_label.setOrigin(m_label.getGlobalBounds().width * 0.5f, m_label.getGlobalBounds().height * 0.5f);
    pos.y = viewport.y * 0.3;
    m_label.setPosition(pos);

    pos.y = viewport.y * 0.5f;
    pos.x = viewport.x * 0.31f;
    size_t i = 0;
    size.x = viewport.x * 0.15f;
    size.y = viewport.y * 0.2f;
    btns.m_play_again.getText().setString("Play Again");
    setupButton(btns.m_play_again, i++, size, pos);
    pos.x += size.x * 0.5f + 10;
    btns.m_save_replay.getText().setString("Save Replay");
    setupButton(btns.m_save_replay, i++, size, pos);
    pos.x += size.x * 0.5f + 10;
    btns.m_save_board.getText().setString("Save Board");
    setupButton(btns.m_save_board, i++, size, pos);
    pos.x += size.x * 0.5f + 10;
    btns.m_mainmenu.getText().setString("MainMenu");
    setupButton(btns.m_mainmenu, i++, size, pos);

    btns.m_mainmenu.setAction([&]() { t_change_scene(new Scene::MainMenu(render, t_quit, t_change_scene)); });
}

auto Victory::draw(sf::RenderTarget &target, sf::RenderStates states) const -> void {
    target.draw(m_bg);
    target.draw(m_label);
    target.draw(btns.m_play_again);
    target.draw(btns.m_save_replay);
    target.draw(btns.m_save_board);
    target.draw(btns.m_mainmenu);
}
auto Victory::handleInput(sf::Event event) -> void {
    btns.m_play_again.handleEvent(event);
    btns.m_save_replay.handleEvent(event);
    btns.m_save_board.handleEvent(event);
    btns.m_mainmenu.handleEvent(event);
}

auto Victory::setupButton(Button &btn, size_t index, sf::Vector2f size, sf::Vector2f pos) -> void {
    sf::RectangleShape *bg = new sf::RectangleShape(size);

    bg->setFillColor(sf::Color::Black);
    btn.getText().setFillColor(sf::Color::White);
    if (index % 2 == 0) {
        bg->setFillColor(sf::Color::White);
        btn.getText().setFillColor(sf::Color::Black);
    }

    bg->setOrigin(size.x * 0.5f, size.y * 0.5f);
    bg->setPosition(pos);
    btn.getText().setCharacterSize(30);
    btn.getText().setFont(AssetManager::GetInstance().getFont("MainMenu.Button"));
    btn.getText().setOrigin(m_label.getGlobalBounds().width * 0.5f, m_label.getGlobalBounds().height * 0.5f);
    btn.setPosition(pos);
}