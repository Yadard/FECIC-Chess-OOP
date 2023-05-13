// clang-format off
#include "pch.hpp"
// clang-format on
#include "Scenes/MainMenu/MainMenu.hpp"

static void setSpriteSize(sf::Sprite &sprite, sf::Vector2f size) {
    float factor;
    if (sprite.getTextureRect().width > sprite.getTextureRect().height) {
        factor = size.x / sprite.getTextureRect().width;
    } else {
        factor = size.y / sprite.getTextureRect().height;
    }
    sprite.setScale(factor, factor);
}

void quit(sf::RenderWindow &render) { render.close(); }

Scene::MainMenu::MainMenu(sf::RenderWindow &render, std::function<void()> t_quit, std::function<void(IScene *)> t_change_scene)
    : IScene(render, t_quit, t_change_scene), m_play_popup(render, t_quit, t_change_scene, m_ugly_fix),
      m_replay_popup(render, [this](const Replay &replay) { m_change_scene(new Scene::ReplayPlayer(m_render, m_quit, m_change_scene, replay)); }) {
    m_mouse_hitbox.width = 4;
    m_mouse_hitbox.height = 4;

    sf::Vector2f viewport((float)render.getSize().x, (float)render.getSize().y);
    m_background.setSize(viewport);
    m_background.setPosition({0, 0});
    m_background.setTexture(&AssetManager::GetInstance().getTexture("MainMenu.BG"));
    m_replay_popup.setBackground(m_background);

    m_tile_size = sf::Vector2f(viewport.x / 8.0, viewport.y / 8.0);
    m_play_btn.getText().setString("Play");
    m_replay_btn.getText().setString("Replay");
    m_quit_btn.getText().setString("Quit");

    m_selected_highlight.setSize(m_tile_size);
    m_selected_highlight.setFillColor(sf::Color::Transparent);
    m_selected_highlight.setOutlineColor(sf::Color(255, 0, 0));
    m_selected_highlight.setOutlineThickness(-5);

    sf::Vector2f pos(512.0, 480.0);

    m_piece.setTexture(AssetManager::GetInstance().getTexture("MainMenu.Piece"), true);
    setSpriteSize(m_piece, {100.0, 100.0});
    m_piece.setOrigin(50, 50);

    setupButtons(m_play_btn, pos);
    pos.y += m_tile_size.y;
    m_selected_highlight.setPosition({pos.x - m_tile_size.x, pos.y});
    m_piece_origin_pos = sf::Vector2f((pos.x - m_tile_size.x) + m_tile_size.x / 2.0, pos.y + m_tile_size.y / 2.0);
    m_piece.setPosition(m_piece_origin_pos);
    setupButtons(m_replay_btn, pos);
    pos.y += m_tile_size.y;
    setupButtons(m_quit_btn, pos);

    m_move_sfx.setBuffer(AssetManager::GetInstance().getSFX("MainMenu.PieceMove"));
    m_logo.setPosition({viewport.x / 2.0f, m_tile_size.y * 1.5f - 20.0f});

    m_play_btn.setAction([this]() {
        m_move_sfx.play();
        m_play_btn.hide();
        m_replay_btn.deactivate();
        m_quit_btn.deactivate();
        m_selected_highlight.setOutlineColor(sf::Color::Transparent);
        m_piece.move({m_tile_size.x, -m_tile_size.y});
        m_play_popup.show();
        return true;
    });
    m_replay_btn.setAction([this]() {
        m_move_sfx.play();
        m_replay_btn.hide();
        m_play_btn.deactivate();
        m_quit_btn.deactivate();
        m_selected_highlight.setOutlineColor(sf::Color::Transparent);
        m_piece.move({m_tile_size.x, 0});
        m_replay_popup.show();
        return true;
    });
    m_quit_btn.setAction([this, &render]() {
        m_move_sfx.stop();
        m_quit();
        return true;
    });
}

auto Scene::MainMenu::setupButtons(Button &btn, sf::Vector2f position) -> void {
    btn.getText().setFont(AssetManager::GetInstance().getFont("MainMenu.Button"));
    uint32_t font_size = 45;
    btn.getText().setFillColor(sf::Color::White);
    btn.getText().setCharacterSize(font_size);

    sf::RectangleShape *bg = new sf::RectangleShape(m_tile_size);
    bg->setPosition(position);
    btn.getText().setPosition(position);
    btn.setSize(m_tile_size);
    position.x += (float)bg->getSize().x / 2.0;
    position.y += (float)bg->getSize().y / 2.0;
    position.y -= btn.getText().getCharacterSize() / 2.0;
    position.x -= (float)btn.getText().getGlobalBounds().width / 2.0;
    btn.getText().setPosition(position);
    btn.setHitbox(bg->getGlobalBounds());
    bg->setFillColor(sf::Color(0, 255, 0, 150));
    btn.setBackground(bg);
}

auto Scene::MainMenu::update(sf::RenderWindow &render) -> void { draw(render); }
auto Scene::MainMenu::draw(sf::RenderWindow &render) -> void {
    render.draw(m_background);
    render.draw(m_piece);
    render.draw(m_selected_highlight);

    this->m_logo.draw(render);
    render.draw(m_play_btn);
    render.draw(m_replay_btn);
    render.draw(m_quit_btn);

    render.draw(m_replay_popup);
    m_play_popup.draw(render);
}

auto Scene::MainMenu::handle_input(std::function<void(Scene::IScene *)> change_scene, std::function<void()> quit, sf::Event event) -> void {
    if (m_play_popup.isVisible()) {
        m_play_popup.handleInput(change_scene, event, m_mouse_hitbox);
        return;
    } else if (m_replay_popup.isVisible()) {
        m_replay_popup.handleInput(event);
        return;
    } else {
        m_piece.setPosition(m_piece_origin_pos);
        m_play_btn.show();
        m_replay_btn.show();
        m_quit_btn.show();
        m_selected_highlight.setOutlineColor(sf::Color(255, 0, 0));
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Right) {
            m_piece.setPosition(m_piece_origin_pos);
            m_play_btn.show();
            m_replay_btn.show();
            m_quit_btn.show();
            m_selected_highlight.setOutlineColor(sf::Color(255, 0, 0));
        }
    }
    m_play_btn.handleEvent(event);
    m_replay_btn.handleEvent(event);
    m_quit_btn.handleEvent(event);
}
