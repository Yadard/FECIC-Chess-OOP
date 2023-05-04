#include "Game.hpp"

static sf::Texture generate_pattern(sf::Vector2f rect_size, sf::Vector2u amount, sf::Color primary_color = sf::Color(255, 213, 153),
                                    sf::Color secondary_color = sf::Color(177, 110, 65));

Game::Game(sf::VideoMode t_render, const char *title) : m_render(t_render, title) {
    m_change_scene = std::bind(&Game::change_scene, this, std::placeholders::_1);
    m_quit = std::bind(&Game::quit, this);
}

auto Game::loadAssets() -> void {
    sf::Vector2f viewport(m_render.getSize().x, m_render.getSize().y);
    AssetManager::GetInstance().registerTexture("MainMenu.BG", generate_pattern(viewport, {12, 12}));
    sf::Texture texture;
    texture.loadFromFile("./../assets/Paint_bg.png");
    AssetManager::GetInstance().registerTexture("MainMenu.ButtonBG", texture);

    sf::Font font;
    font.loadFromFile("./../assets/PIXABLER.ttf");
    AssetManager::GetInstance().registerFont("MainMenu.Button", font);
}

auto Game::gameloop() -> void {
    m_cur_scene.reset(new Scene::MainMenu(m_render));

    while (m_render.isOpen()) {

        m_render.clear(sf::Color(100, 100, 100));
        if (!drawn) {
            m_cur_scene->draw(m_render);
            drawn = true;
        }
        catchEvents();

        m_cur_scene->update(m_render);
        while (!m_event_queue.empty()) {
            if (m_event_queue.front().type == sf::Event::EventType::Closed) {
                m_render.close();
            }
            m_cur_scene->handle_input(m_change_scene, m_quit, m_event_queue.front());
            m_event_queue.pop();
        }
        m_render.display();
    }
}

auto Game::quit() -> void { m_render.close(); }
auto Game::change_scene(Scene::IScene *new_scene) -> void {
    this->m_cur_scene.reset(new_scene);
    drawn = false;
}

auto Game::catchEvents() -> void {
    if (m_render.isOpen()) {
        sf::Event event;
        while (m_render.pollEvent(event))
            m_event_queue.push(event);
    }
}

static sf::Texture generate_pattern(sf::Vector2f rect_size, sf::Vector2u amount, sf::Color primary_color, sf::Color secondary_color) {
    static sf::RenderTexture rt;
    rt.create(rect_size.x, rect_size.y);

    size_t cols = amount.x;
    size_t rows = amount.y;
    sf::Vector2f size({rect_size.x / (float)cols, rect_size.y / (float)rows});
    rt.clear(sf::Color::White);
    bool color = true;
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            sf::RectangleShape rect;
            rect.setSize(size);
            rect.setPosition({size.x * j, size.y * i});
            sf::Color c;
            if (j % 2 == 0) {
                if (i % 2 == 0) {
                    c = primary_color;
                } else {
                    c = secondary_color;
                }
            } else {
                if (i % 2 == 0) {
                    c = secondary_color;
                } else {
                    c = primary_color;
                }
            }
            rect.setFillColor(c);
            color = !color;
            rt.draw(rect);
        }
    }

    rt.display();
    return rt.getTexture();
}