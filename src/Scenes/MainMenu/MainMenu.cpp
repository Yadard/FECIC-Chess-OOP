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

Scene::MainMenu::MainMenu(sf::RenderWindow &render) : m_play_btn("Play"), m_replay_btn("Replay"), m_quit_btn("Quit"), m_popup(render) {
    m_mouse_hitbox.width = 4;
    m_mouse_hitbox.height = 4;

    sf::Vector2f viewport((float)render.getSize().x, (float)render.getSize().y);
    m_background.setSize(viewport);
    m_background.setPosition({0, 0});
    m_background.setTexture(&AssetManager::GetInstance().getTexture("MainMenu.BG"));

    m_tile_size = sf::Vector2f(viewport.x / 8.0, viewport.y / 8.0);
    m_play_btn.setBGSize(m_tile_size);
    m_replay_btn.setBGSize(m_tile_size);
    m_quit_btn.setBGSize(m_tile_size);
    m_selected_highlight.setSize(m_tile_size);
    m_selected_highlight.setFillColor(sf::Color::Transparent);
    m_selected_highlight.setOutlineColor(sf::Color(255, 0, 0));
    m_selected_highlight.setOutlineThickness(-5);

    sf::Vector2f pos(512.0, 480.0);

    m_piece.setTexture(AssetManager::GetInstance().getTexture("MainMenu.Piece"), true);
    setSpriteSize(m_piece, {100.0, 100.0});
    m_piece.setOrigin(50, 50);

    m_play_btn.setPosition(pos);
    pos.y += m_tile_size.y;
    m_selected_highlight.setPosition({pos.x - m_tile_size.x, pos.y});
    m_piece_origin_pos = sf::Vector2f((pos.x - m_tile_size.x) + m_tile_size.x / 2.0, pos.y + m_tile_size.y / 2.0);
    m_piece.setPosition(m_piece_origin_pos);
    m_replay_btn.setPosition(pos);
    pos.y += m_tile_size.y;
    m_quit_btn.setPosition(pos);

    m_move_sfx.setBuffer(AssetManager::GetInstance().getSFX("MainMenu.PieceMove"));

    m_logo.setPosition({viewport.x / 2.0f, m_tile_size.y * 1.5f - 20.0f});
}

auto Scene::MainMenu::update(sf::RenderWindow &render) -> void { draw(render); }
auto Scene::MainMenu::draw(sf::RenderWindow &render) -> void {
    render.draw(m_background);
    render.draw(m_piece);
    render.draw(m_selected_highlight);

    this->m_logo.draw(render);
    this->m_play_btn.draw(render);
    this->m_replay_btn.draw(render);
    this->m_quit_btn.draw(render);

    this->m_popup.draw(render);
}

auto Scene::MainMenu::handle_input(std::function<void(Scene::IScene *)> change_scene, std::function<void()> quit, sf::Event event) -> void {
    if (m_popup.isVisible()) {
        if (m_popup.handleInput(change_scene, event, m_mouse_hitbox)) {
            m_piece.setPosition(m_piece_origin_pos);
            m_play_btn.show();
            m_replay_btn.show();
            m_quit_btn.show();
            m_selected_highlight.setOutlineColor(sf::Color(255, 0, 0));
        }
        return;
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Right) {
            m_piece.setPosition(m_piece_origin_pos);
            m_play_btn.show();
            m_replay_btn.show();
            m_quit_btn.show();
            m_selected_highlight.setOutlineColor(sf::Color(255, 0, 0));
        }

        if (m_play_btn.clicked(event, m_mouse_hitbox)) {
            m_move_sfx.play();
            m_play_btn.hide();
            m_replay_btn.deactivate();
            m_quit_btn.deactivate();
            m_selected_highlight.setOutlineColor(sf::Color::Transparent);
            m_piece.move({m_tile_size.x, -m_tile_size.y});
            m_popup.show();
            return;
        }
        if (m_replay_btn.clicked(event, m_mouse_hitbox)) {
            m_move_sfx.play();
            m_replay_btn.hide();
            m_play_btn.deactivate();
            m_quit_btn.deactivate();
            m_selected_highlight.setOutlineColor(sf::Color::Transparent);
            m_piece.move({m_tile_size.x, 0});
            return;
        }
        if (m_quit_btn.clicked(event, m_mouse_hitbox)) {
            m_move_sfx.stop();
            quit();
            return;
        }
    }
}
