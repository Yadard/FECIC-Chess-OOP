#include "Scenes/Chess/MatchResult/Victory.hpp"

Victory::Victory(sf::Vector2f viewport, std::function<void()> t_mainmenu, std::function<void()> t_play_again, Preset preset, bool new_board, Replay replay,
                 Team winner)
    : MatchResult(viewport, t_mainmenu, t_play_again, preset, new_board, replay) {
    if (winner == Team::WHITE)
        m_label.setString("WHITE Won");
    else
        m_label.setString("BLACK Won");

    m_label.setPosition({0, 0});
    m_label.setCharacterSize(120);
    m_label.setFont(AssetManager::GetInstance().getFont("MainMenu.Button"));
    m_label.setFillColor(sf::Color(220, 220, 220));

    sf::Vector2f pos = m_bg.getPosition();
    pos.x += m_bg.getSize().x * 0.5f;
    pos.y += m_bg.getSize().y * 0.15f;
    m_label.setPosition(pos);
    m_label.move(-m_label.getGlobalBounds().width * 0.5f, 0);
}

auto Victory::draw(sf::RenderTarget &target, sf::RenderStates states) const -> void {
    target.draw(m_bg);
    target.draw(m_label);
    target.draw(m_btns.play_again);
    if (m_new_board)
        target.draw(m_btns.save_board);
    target.draw(m_btns.save_replay);
    target.draw(m_btns.mainmenu);
    target.draw(m_input);
}

auto Victory::update() -> void { m_input.update(); }

auto Victory::handleInput(sf::Event event) -> void {
    if (m_input.visible()) {
        m_input.handleEvent(event);
        return;
    }

    m_btns.play_again.handleEvent(event);
    m_btns.save_board.handleEvent(event);
    m_btns.save_replay.handleEvent(event);
    m_btns.mainmenu.handleEvent(event);
}