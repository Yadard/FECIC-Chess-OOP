#include "Scenes/MainMenu/Popup/Play.hpp"
#include "Scenes/Chess/Chess.hpp"

PlayPopup::PlayPopup(sf::RenderWindow &t_render, std::function<void()> t_quit, std::function<void(Scene::IScene *)> t_change_scene, bool &ugly_fix) {
    m_play_btn.getText().setString("Play");
    m_close_btn.getText().setString("Close");
    m_show = false;
    sf::Vector2f viewport(t_render.getSize().x, t_render.getSize().y);
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
    m_piece_selector.setSize(size);

    loadPresets();

    pos.y = m_background.getGlobalBounds().top + m_background.getGlobalBounds().height - 60;
    pos.x = m_background.getGlobalBounds().left + m_background.getGlobalBounds().width * 0.8;
    setupButtons(m_play_btn, pos);
    pos.x = m_background.getGlobalBounds().left + m_background.getGlobalBounds().width * 0.2;
    setupButtons(m_close_btn, pos);

    m_play_btn.setAction([&, this]() {
        hide();
        t_change_scene(new Scene::Chess(t_render, t_quit, t_change_scene, this->m_current_preset, this->m_selected_preset == nullptr));
    });
    m_close_btn.setAction([&]() { hide(); });
}

auto PlayPopup::isVisible() -> bool { return m_show; }

auto PlayPopup::setupButtons(Button &btn, sf::Vector2f pos) -> void {
    btn.getText().setFont(AssetManager::GetInstance().getFont("MainMenu.Button"));
    btn.getText().setFillColor(sf::Color::Black);
    btn.getText().setCharacterSize(40);

    m_background.setSize({140, 60});
    m_background.setOrigin({70, 30});
    m_background.setPosition(pos);

    btn.getText().setOrigin(btn.getText().getLocalBounds().width * 0.5f, btn.getText().getLocalBounds().height * 0.8f);
    btn.getText().setPosition(pos);
}

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

auto PlayPopup::applyPreset() -> void {
    m_preview.resize(m_selected_preset->preset.getBoardSize());
    for (auto &piece : m_selected_preset->preset.getPiecesInfo()) {
        m_preview.addPiece(AssetManager::GetInstance().piece_factory.createPiece(piece.name, piece.team, piece.board_pos), piece.board_pos);
    }
}

auto PlayPopup::draw(sf::RenderWindow &render) -> void {
    if (!m_show)
        return;

    render.draw(m_background);
    render.draw(m_presets_bg);
    render.draw(m_play_btn);
    render.draw(m_close_btn);
    m_preview.draw(render);

    if (m_selected_preset)
        m_selected_preset->background.setFillColor(sf::Color(0, 255, 0, 150));

    for (auto &preset : m_presets)
        preset.draw(render);

    render.draw(m_piece_selector);
}

auto PlayPopup::handleInput(std::function<void(Scene::IScene *)> change_scene, sf::Event event, sf::FloatRect mouse) -> bool {
    Tile *tile;
    auto callback = [&](std::string name, Team team, Tile *tile) {
        Piece *p = AssetManager::GetInstance().piece_factory.createPiece(name, team, tile->board_pos);
        Preset::PieceInfo pieceinfo;
        pieceinfo.name = name;
        pieceinfo.team = team;
        pieceinfo.board_pos = tile->board_pos;
        m_current_preset.addPiecesInfo(pieceinfo);
        m_current_preset.setName("");
        m_selected_preset = nullptr;
        m_preview.addPiece(p, tile->board_pos);
    };
    switch (event.type) {
    case sf::Event::MouseButtonPressed:
        mouse.left = event.mouseButton.x;
        mouse.top = event.mouseButton.y;
        if (event.mouseButton.button == sf::Mouse::Right) {
            if (m_piece_selector.visible())
                m_piece_selector.hide();
            else if ((tile = m_preview.handleMouseClick(mouse))) {
                m_selected_preset = nullptr;
                if (tile->getPiece() != tile->INVALID_INDEX)
                    m_current_preset.erasePiecesInfo(tile->getPiece());
                m_preview.removePiece(tile->board_pos);
            }

            return true;
        }
        std::cout << '(' << event.mouseButton.x << ',' << event.mouseButton.y << ")\n";
        if (m_piece_selector.handleMouseLeft(callback, mouse)) {

            return true;
        } else {
            if (m_piece_selector.visible()) {
                m_piece_selector.hide();
                return true;
            }
        }
        if ((tile = m_preview.handleMouseClick(mouse))) {

            m_piece_selector.show({mouse.left, mouse.top}, *tile, tile->original_color);
        } else {
            for (auto &preset : m_presets) {
                if (preset.clicked(mouse)) {
                    m_preview.clearBoard();
                    m_selected_preset = &preset;
                    m_current_preset = preset.preset;
                    applyPreset();
                }
            }
        }
        break;

    case sf::Event::KeyPressed: {
        auto new_size = m_current_preset.getBoardSize();
        if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
            m_preview.resize(m_preview.getBoardDimensions().x + 1, m_preview.getBoardDimensions().y);
            ++new_size.x;
            m_current_preset.setBoardSize(new_size);
            m_current_preset.setName("");
            m_selected_preset = nullptr;

        } else if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
            m_preview.resize(m_preview.getBoardDimensions().x - 1, m_preview.getBoardDimensions().y);
            --new_size.x;
            m_current_preset.setBoardSize(new_size);
            m_current_preset.setName("");
            m_selected_preset = nullptr;

        } else if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
            m_preview.resize(m_preview.getBoardDimensions().x, m_preview.getBoardDimensions().y + 1);
            ++new_size.y;
            m_current_preset.setBoardSize(new_size);
            m_current_preset.setName("");
            m_selected_preset = nullptr;

        } else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
            m_preview.resize(m_preview.getBoardDimensions().x, m_preview.getBoardDimensions().y - 1);
            --new_size.y;
            m_current_preset.setBoardSize(new_size);
            m_current_preset.setName("");
            m_selected_preset = nullptr;
        } else if (event.key.code == sf::Keyboard::Escape) {
            hide();
            return true;
        }

        break;
    }
    default:
        break;
    }

    return false;
}

auto PlayPopup::hide() -> void { m_show = false; }
auto PlayPopup::show() -> void { m_show = true; }

PlayPopup::__Btton::__Btton(std::string label_text) {

    m_label.setString(label_text);
    m_label.setFont(AssetManager::GetInstance().getFont("MainMenu.__Btton"));
    m_label.setFillColor(sf::Color::Black);
    m_label.setCharacterSize(40);
}

auto PlayPopup::__Btton::clicked(sf::Event event, sf::FloatRect mouse) -> bool {
    mouse.left = event.mouseButton.x;
    mouse.top = event.mouseButton.y;

    return m_background.getGlobalBounds().intersects(mouse);
}

auto PlayPopup::__Btton::setPosition(sf::Vector2f position) -> void {
    m_background.setSize({140, 60});
    m_background.setOrigin({70, 30});
    m_background.setPosition(position);

    m_label.setOrigin(m_label.getLocalBounds().width * 0.5f, m_label.getLocalBounds().height * 0.8f);
    m_label.setPosition(position);
}

auto PlayPopup::__Btton::draw(sf::RenderWindow &render) -> void {
    render.draw(m_background);
    render.draw(m_label);
}

PlayPopup::PresetEntry::PresetEntry(const Preset &t_preset, std::string str) : preset(t_preset) {
    btn.setString(str);
    btn.setFont(AssetManager::GetInstance().getFont("MainMenu.__Btton"));
    btn.setFillColor(sf::Color::Black);
    btn.setCharacterSize(20);
}

auto PlayPopup::PresetEntry::draw(sf::RenderWindow &render) -> void {
    render.draw(background);
    render.draw(btn);
    render.draw(line);

    background.setFillColor(sf::Color::Transparent);
}
auto PlayPopup::PresetEntry::clicked(sf::FloatRect mouse) -> bool { return background.getGlobalBounds().intersects(mouse); }

PlayPopup::PieceSelector::PieceSelector() : m_shape(sf::Triangles, 9), m_entry_size(40, 40) {
    auto &piece_factory = AssetManager::GetInstance().piece_factory;
    for (auto it = piece_factory.cbegin(); it != piece_factory.cend(); it++) {
        m_white_sprites.emplace_back(it->first, *it->second.piece_white_texture, m_entry_size);
        m_black_sprites.emplace_back(it->first, *it->second.piece_black_texture, m_entry_size);
    }
}

auto PlayPopup::PieceSelector::show(sf::Vector2f pos, Tile &tile, sf::Color origin_tile_color) -> void {
    m_tile_color = origin_tile_color;
    m_position = pos;
    m_tile = &tile;

    generateShape(pos, m_rect, m_trig, sf::Color(255, 255, 255, 255));
    sf::FloatRect bounds = m_shape.getBounds();
    pos = sf::Vector2f(bounds.left + 5, bounds.top + 5);
    sf::Vector2f size = m_entry_size;

    auto next_pos = [&]() mutable {
        pos.x += size.x + 5;
        if (pos.x + size.x > bounds.left + bounds.width) {
            pos.x = bounds.left + 5;
            pos.y += size.y + 5;
        }
    };

    for (auto &white : m_white_sprites) {
        white.setColor(m_tile_color);
        white.setPosition(pos);
        next_pos();
    }

    for (auto &black : m_black_sprites) {
        black.setColor(m_tile_color);
        black.setPosition(pos);
        next_pos();
    }

    m_show = true;
}
auto PlayPopup::PieceSelector::hide() -> void { m_show = false; }
auto PlayPopup::PieceSelector::visible() -> bool { return m_show; }

auto PlayPopup::PieceSelector::handleMouseLeft(std::function<void(std::string, Team, Tile *)> add_piece, sf::FloatRect mouse) -> bool {
    sf::FloatRect bounds = m_shape.getBounds();
    if (!bounds.intersects(mouse) || !m_show)
        return false;

    for (size_t i = 0; i < std::max(m_white_sprites.size(), m_black_sprites.size()); i++) {
        if (i < m_white_sprites.size()) {
            if (m_white_sprites[i].hitbox.getGlobalBounds().intersects(mouse)) {
                add_piece(m_white_sprites[i].piece_name, Team::WHITE, m_tile);
            }
        }
        if (i < m_black_sprites.size()) {
            if (m_black_sprites[i].hitbox.getGlobalBounds().intersects(mouse)) {
                add_piece(m_black_sprites[i].piece_name, Team::BLACK, m_tile);
            }
        }
    }

    m_show = false;
    return true;
}

auto PlayPopup::PieceSelector::generateShape(sf::Vector2f pos, sf::Vector2f rect, sf::Vector2f trig, sf::Color color) -> void {
    size_t i = 0;
    m_shape[i].position = pos;
    m_shape[i++].color = color;

    pos.x -= trig.x * 0.5f;
    pos.y -= trig.y;
    m_shape[i].position = pos;
    m_shape[i++].color = color;

    pos.x += trig.x;
    m_shape[i].position = pos;
    m_shape[i++].color = color;

    pos.x += rect.x * 0.5f - trig.x * 0.5;
    m_shape[i].position = pos;
    m_shape[i++].color = color;

    pos.y -= rect.y;
    m_shape[i].position = pos;
    m_shape[i++].color = color;

    pos.y += rect.y;
    pos.x -= rect.x;
    m_shape[i].position = pos;
    m_shape[i++].color = color;

    m_shape[i].position = m_shape[i - 1].position;
    m_shape[i++].color = color;

    pos.y -= rect.y;
    m_shape[i].position = pos;
    m_shape[i++].color = color;

    m_shape[i].position = m_shape[i - 4].position;
    m_shape[i++].color = color;
}
auto PlayPopup::PieceSelector::setSize(sf::Vector2f board_size) -> void {
    size_t entry_amount = m_white_sprites.size() + m_black_sprites.size();
    size_t amount_of_rows = entry_amount / ((board_size.x - 10) / (m_entry_size.x + 5));

    m_rect.x = board_size.x;
    m_rect.y = amount_of_rows * 90;

    m_trig.x = board_size.x * 0.2f;
    m_trig.y = board_size.x * 0.45f;
}

auto PlayPopup::PieceSelector::draw(sf::RenderTarget &target, sf::RenderStates states) const -> void {
    if (!m_show)
        return;
    target.draw(m_shape);
    for (auto &entry : m_white_sprites)
        target.draw(entry);
    for (auto &entry : m_black_sprites)
        target.draw(entry);
}

PlayPopup::PieceSelector::Entry::Entry(std::string piece, const sf::Texture &texture, sf::Vector2f size) {
    piece_name = piece;
    sprite.setTexture(texture, true);
    sprite.setOrigin(texture.getSize().x * 0.5f, texture.getSize().y * 0.5f);

    hitbox.setOutlineThickness(2);
    hitbox.setOutlineColor(sf::Color::Black);
    hitbox.setSize(size);
}

auto PlayPopup::PieceSelector::Entry::draw(sf::RenderTarget &target, sf::RenderStates states) const -> void {
    target.draw(hitbox);
    target.draw(sprite);
}
auto PlayPopup::PieceSelector::Entry::setPosition(sf::Vector2f pos) -> void {
    sprite.setPosition(pos.x + hitbox.getSize().x * 0.5f, pos.y + hitbox.getSize().y * 0.5f);
    hitbox.setPosition(pos);
}

auto PlayPopup::PieceSelector::Entry::setColor(sf::Color origin_tile_color) -> void { hitbox.setFillColor(origin_tile_color); }