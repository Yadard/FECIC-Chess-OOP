#include "Scenes/MainMenu/Popup/Play.hpp"

PlayPopup::PlayPopup(sf::RenderWindow &render) : m_play_btn("Play"), m_close_btn("Close") {
    m_show = false;
    sf::Vector2f viewport(render.getSize().x, render.getSize().y);
    viewport.x *= 0.5f;
    viewport.y *= 0.8f;

    m_background.setSize(viewport);
    m_background.setOrigin({viewport.x * 0.5f, viewport.y * 0.5f});
    m_background.setFillColor(sf::Color::Black);

    m_background.setPosition(viewport.x, viewport.y * 0.7f);

    sf::Vector2f base(viewport.x * 0.5f, viewport.y * 0.20f);
    m_presets_bg.setSize({viewport.x * 0.4f, viewport.y * 0.80f});
    m_presets_bg.setPosition(base.x * 1.05f, base.y * 1.1f);

    sf::Vector2f size(viewport.x * 0.40f, viewport.y * 0.40f);
    sf::Vector2f pos(base.x + viewport.x * 0.50f, base.y + viewport.y * 0.42f);
    m_preview.makeBoard(pos, size, {5, 5});

    loadPresets();

    pos.y = m_background.getGlobalBounds().top + m_background.getGlobalBounds().height - 60;
    pos.x = m_background.getGlobalBounds().left + m_background.getGlobalBounds().width * 0.8;
    m_play_btn.setPosition(pos);
    pos.x = m_background.getGlobalBounds().left + m_background.getGlobalBounds().width * 0.2;
    m_close_btn.setPosition(pos);
}

auto PlayPopup::isVisible() -> bool { return m_show; }

auto PlayPopup::loadPresets() -> void {
    sf::Vector2f size = m_presets_bg.getSize();
    sf::Vector2f pos = m_presets_bg.getPosition();
    auto &presets = AssetManager::GetInstance();

    pos.x += size.x * 0.05;
    for (auto it = presets.presetCBegin(); it != presets.presetCEnd(); it++) {
        m_presets.emplace_back(it->second, it->second.getName());
        m_presets.back().btn.setPosition(pos);
        m_presets.back().background.setPosition(m_presets_bg.getPosition().x, pos.y);
        const float height = size.y * 0.0175f + m_presets.back().btn.getLocalBounds().height;
        m_presets.back().background.setSize({m_presets_bg.getSize().x, height});
        m_presets.back().background.setFillColor(sf::Color::Transparent);
        pos.y += height;
        m_presets.back().line.setPosition(pos.x, pos.y);
        m_presets.back().line.setSize({size.x * 0.90f, 1.0});
        m_presets.back().line.setFillColor(sf::Color(0, 0, 0, 180));
    }
}

auto PlayPopup::applyPreset() -> void { m_preview.resize(m_selected_preset->preset.getBoardSize()); }

auto PlayPopup::draw(sf::RenderWindow &render) -> void {
    if (!m_show)
        return;

    render.draw(m_background);
    render.draw(m_presets_bg);
    m_play_btn.draw(render);
    m_close_btn.draw(render);
    m_preview.draw(render);

    if (m_selected_preset)
        m_selected_preset->background.setFillColor(sf::Color(0, 255, 0, 150));

    for (auto &preset : m_presets)
        preset.draw(render);
}

auto PlayPopup::handleInput(std::function<void(Scene::IScene *)> change_scene, sf::Event event, sf::FloatRect mouse) -> bool {
    Tile *tile;
    switch (event.type) {
    case sf::Event::MouseButtonPressed:
        mouse.left = event.mouseButton.x;
        mouse.top = event.mouseButton.y;
        if (m_close_btn.clicked(event, mouse)) {
            hide();
            return true;
        } else if (m_play_btn.clicked(event, mouse)) {
            hide();
            return true;
        } else if ((tile = m_preview.handleMouseClick(mouse))) {

        } else {
            for (auto &preset : m_presets) {
                if (preset.clicked(mouse)) {
                    m_selected_preset = &preset;
                    applyPreset();
                }
            }
        }
        break;

    case sf::Event::KeyPressed:
        if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
            m_preview.resize(m_preview.getBoardDimensions().x + 1, m_preview.getBoardDimensions().y);
            m_selected_preset = nullptr;

        } else if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
            m_preview.resize(m_preview.getBoardDimensions().x - 1, m_preview.getBoardDimensions().y);
            m_selected_preset = nullptr;

        } else if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
            m_preview.resize(m_preview.getBoardDimensions().x, m_preview.getBoardDimensions().y + 1);
            m_selected_preset = nullptr;

        } else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
            m_preview.resize(m_preview.getBoardDimensions().x, m_preview.getBoardDimensions().y - 1);
            m_selected_preset = nullptr;
        } else if (event.key.code == sf::Keyboard::Escape) {
            hide();
            return true;
        }

        break;

    default:
        break;
    }

    return false;
}

auto PlayPopup::hide() -> void { m_show = false; }
auto PlayPopup::show() -> void { m_show = true; }

PlayPopup::Button::Button(std::string label_text) {

    m_label.setString(label_text);
    m_label.setFont(AssetManager::GetInstance().getFont("MainMenu.Button"));
    m_label.setFillColor(sf::Color::Black);
    m_label.setCharacterSize(40);
}

auto PlayPopup::Button::clicked(sf::Event event, sf::FloatRect mouse) -> bool {
    mouse.left = event.mouseButton.x;
    mouse.top = event.mouseButton.y;

    return m_background.getGlobalBounds().intersects(mouse);
}

auto PlayPopup::Button::setPosition(sf::Vector2f position) -> void {
    m_background.setSize({140, 60});
    m_background.setOrigin({70, 30});
    m_background.setPosition(position);

    m_label.setOrigin(m_label.getLocalBounds().width * 0.5f, m_label.getLocalBounds().height * 0.8f);
    m_label.setPosition(position);
}

auto PlayPopup::Button::draw(sf::RenderWindow &render) -> void {
    render.draw(m_background);
    render.draw(m_label);
}