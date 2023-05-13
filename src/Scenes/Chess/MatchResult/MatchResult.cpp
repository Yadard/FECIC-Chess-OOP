#include "Scenes/Chess/MatchResult/MatchResult.hpp"

auto savePreset(Preset preset) -> void {
    preset.save();
    AssetManager::GetInstance().registerPreset(preset.getName(), preset);
}
auto saveReplay(Replay replay) -> void {
    replay.save();
    AssetManager::GetInstance().registerReplay(replay.getName(), replay);
}

MatchResult::MatchResult(sf::Vector2f viewport, std::function<void()> t_mainmenu, std::function<void()> t_play_again, Preset preset, bool new_board,
                         Replay replay)
    : mainmenu(t_mainmenu), play_again(t_play_again), preset(preset), replay(replay), m_new_board(new_board) {
    sf::Vector2f size(viewport.x * 0.8f, viewport.y * 0.5f);
    sf::Vector2f pos(viewport.x * 0.1f, viewport.y * 0.15f);

    m_bg.setSize(size);
    m_bg.setPosition(pos);
    m_bg.setFillColor(sf::Color(70, 55, 48, 200));

    sf::Vector2f input_pos = pos;
    sf::Vector2f input_size(size.x * 0.8f, size.y * 0.5f);
    input_pos.x += size.x * 0.5f;
    input_pos.y += size.y * 0.5f;
    input_pos.x -= input_size.x * 0.5f;
    input_pos.y -= input_size.y * 0.5f;

    m_input.setDimensions(input_pos, input_size);
    m_input.setInputLimit(20);

    sf::Vector2f btn_size = size;
    float spacing = 1;
    if (!new_board) {
        btn_size.x = size.x * 0.333f;
        spacing *= btn_size.x * 0.20f;
        btn_size.x = (size.x - spacing * 4.f) * 0.333f;
    } else {
        btn_size.x = size.x * 0.25f;
        spacing *= btn_size.x * 0.2f;
        btn_size.x = (size.x - spacing * 5.f) * 0.25f;
    }
    btn_size.y = size.y * 0.1f;

    pos.y += size.y * 0.95f - btn_size.y;
    setupButtons(pos, btn_size, spacing, new_board);
}

auto MatchResult::setupButtons(sf::Vector2f pos, sf::Vector2f size, float spacing, bool show_save_board) -> void {
    pos.x += spacing;
    m_btns.play_again.getText().setString("Play Again");
    m_btns.play_again.setAction([this]() {
        this->play_again();
        return true;
    });
    setupButton(m_btns.play_again, size, pos);
    pos.x += size.x + spacing;
    if (show_save_board) [[unlikely]] {
        m_btns.save_board.setAction([this]() {
            m_input.setInputLabel("Board name");
            m_input.setMainLabel("Enter the name for the board:");
            m_input.show();
            m_input.setSubmitCallback([this](std::string str) {
                preset.setName(str);
                ThreadDispatcher::GetInstance().launchThread(new sf::Thread(&savePreset, preset));
            });
            return true;
        });
        m_btns.save_board.getText().setString("Save Board");
        setupButton(m_btns.save_board, size, pos);
        pos.x += size.x + spacing;
    }

    m_btns.save_replay.setAction([this]() {
        m_input.setInputLabel("Replay name");
        m_input.setMainLabel("Enter the name for the replay:");
        m_input.show();
        m_input.setSubmitCallback([this](std::string str) {
            replay.setName(str);
            ThreadDispatcher::GetInstance().launchThread(new sf::Thread(&saveReplay, replay));
        });
        return true;
    });
    m_btns.save_replay.getText().setString("Save Replay");
    setupButton(m_btns.save_replay, size, pos);
    pos.x += size.x + spacing;
    m_btns.mainmenu.setAction([this]() {
        mainmenu();
        return true;
    });
    m_btns.mainmenu.getText().setString("MainMenu");
    setupButton(m_btns.mainmenu, size, pos);
}

auto MatchResult::setupButton(Button &btn, sf::Vector2f size, sf::Vector2f pos) -> void {
    sf::RectangleShape *bg = new sf::RectangleShape();
    bg->setFillColor(sf::Color(83, 134, 228));
    bg->setSize(size);
    bg->setPosition(pos);

    btn.getText().setPosition(pos.x, pos.y);
    btn.getText().setCharacterSize(25);
    btn.getText().setFont(AssetManager::GetInstance().getFont("MainMenu.Button"));
    btn.getText().move(size.x * 0.5f, size.y * 0.5f);
    auto foo = btn.getText().getGlobalBounds();
    btn.getText().move(-foo.width * 0.5f, -foo.height * 0.7f);
    btn.setBackground(bg);
    btn.setHitbox(bg->getGlobalBounds());
}