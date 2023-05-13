#include "Scenes/MainMenu/Popup/Replay.hpp"

ReplayPopup::ReplayPopup(sf::RenderWindow &t_render, std::function<void(const Replay &)> t_replay_player) : m_replay_player(t_replay_player) {

    sf::Vector2f viewport(t_render.getSize().x, t_render.getSize().y);

    sf::Vector2f pos = viewport;
    pos.y *= 0.4f;
    pos.x *= 0.08f;
    sf::Vector2f size = viewport;
    size.y *= 0.15f;
    size.x *= 0.05f;

    m_left_arrow.setDimensions(pos, size, false);
    pos.x = viewport.x * 0.93f;
    m_right_arrow.setDimensions(pos, size, true);

    size = viewport;
    size.x *= 0.5f;
    size.y *= 0.6f;
    pos = viewport;
    pos.x *= 0.5f;
    pos.y *= 0.5f;
    m_preview.makeBoard({pos.x - size.x * 0.5f, pos.y - size.y * 0.5f}, size, {8, 8});

    pos.y += size.y * 0.425f + 5;

    pos.x -= size.x * 0.625f;
    loadReplays();
    m_current_replay = m_replays.rbegin();
    if (m_current_replay != m_replays.rend())
        applyReplay(*m_current_replay);

    m_replay_name.setDimensions(pos, {size.x * 1.25f, viewport.y * 0.15f});
    m_replay_name.getText().move(0, -15);
    m_replay_name.getText().setCharacterSize(40);
    m_replay_name.getText().setFillColor(sf::Color::White);
    m_replay_name.setMainLabel("");
    m_replay_name.setInputLabel("");
    sf::Color c = m_replay_name.getInputBg().getFillColor();
    c.a = 100;
    m_replay_name.getInputBg().setFillColor(c);
    m_replay_name.setInputLimit(20);
    m_replay_name.getBG().setFillColor(sf::Color::Transparent);
    m_replay_name.show();
    m_replay_name.deactivate();
}

auto ReplayPopup::setBackground(sf::RectangleShape &bg) -> void {
    m_background = bg;
    m_background.setFillColor(sf::Color(0, 0, 0, 200));
}
auto ReplayPopup::hide() -> void { m_show = false; }
auto ReplayPopup::show() -> void { m_show = true; }
auto ReplayPopup::isVisible() const -> bool { return m_show; }

auto ReplayPopup::draw(sf::RenderTarget &target, sf::RenderStates states) const -> void {
    if (!m_show)
        return;

    target.draw(m_background);
    target.draw(m_preview);
    target.draw(m_replay_name);
    target.draw(m_left_arrow);
    target.draw(m_right_arrow);
}

auto deleteReplay(Replay replay) -> void { replay.del(); }

auto ReplayPopup::handleInput(sf::Event event) -> bool {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mouse_pos(event.mouseButton.x, event.mouseButton.y);
        if (m_left_arrow.getHitbox().contains(mouse_pos)) {
            if (m_current_replay != m_replays.crbegin())
                std::advance(m_current_replay, -1);
        } else if (m_right_arrow.getHitbox().contains(mouse_pos)) {
            if (std::next(m_current_replay) != m_replays.crend())
                std::advance(m_current_replay, 1);
        }

    } else if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
        case sf::Keyboard::A:
        case sf::Keyboard::Left:
            if (m_current_replay != m_replays.crbegin())
                std::advance(m_current_replay, -1);
            break;

        case sf::Keyboard::D:
        case sf::Keyboard::Right:
            if (std::next(m_current_replay) != m_replays.crend())
                std::advance(m_current_replay, 1);
            break;

        case sf::Keyboard::Enter:
            m_replay_player(m_current_replay->replay);
            return false;
            break;

        case sf::Keyboard::Escape:
            this->hide();
            return false;
            break;

        case sf::Keyboard::Delete: {
            ThreadDispatcher::GetInstance().launchThread(new sf::Thread(deleteReplay, m_current_replay->replay));
            auto del = m_current_replay;
            if (std::next(m_current_replay) != m_replays.crend())
                std::advance(m_current_replay, 1);
            else
                std::advance(m_current_replay, -1);

            m_replays.erase(del.base());
        } break;

        default:
            break;
        }
    }
    applyReplay(*m_current_replay);

    return false;
}

auto ReplayPopup::loadReplays() -> void {
    const auto &replays = AssetManager::GetInstance().getReplays();
    for (auto it = replays.cbegin(); it != replays.cend(); it++) {
        m_replays.emplace_back(it->second, it->first);
    }
}
auto ReplayPopup::applyReplay(const Entry &entry) -> void {

    m_preview.resize(entry.replay.getBoardSize());
    m_replay_name.setString(entry.name);
    m_preview.clearBoard();

    const auto &pieces = entry.replay.getPiecesInfo();
    auto &piece_maker = AssetManager::GetInstance().piece_factory;
    for (auto it = pieces.cbegin(); it != pieces.cend(); it++) {
        Piece *piece = piece_maker.createPiece(it->name, it->team, it->board_pos);
        m_preview.addPiece(piece, piece->position);
    }
}