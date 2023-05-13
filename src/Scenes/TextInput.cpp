//clang-format off
#include "pch.hpp"
//clang-format on

#include "Scenes/TextInput.hpp"

auto TextInput::setDimensions(sf::Vector2f pos, sf::Vector2f size) -> void {
    m_bg.setPosition(pos);
    m_bg.setSize(size);
    m_bg.setFillColor(sf::Color(22, 21, 48));

    m_input.setCharacterSize(30);
    m_input.setFillColor(sf::Color(236, 235, 242));
    m_input.setString(m_string);
    m_input.setFont(AssetManager::GetInstance().getFont("MainMenu.Button"));

    m_label.setCharacterSize(15);
    m_label.setFillColor(sf::Color(101, 140, 208));
    m_label.setFont(AssetManager::GetInstance().getFont("MainMenu.Button"));

    m_main_label.setCharacterSize(45);
    m_main_label.setFillColor(sf::Color(238, 236, 240));
    m_main_label.setFont(AssetManager::GetInstance().getFont("MainMenu.Button"));

    m_input_bg.setPosition(pos);
    m_main_label.setPosition(pos);
    m_main_label.move(size.x * 0.05f, size.y * 0.05f);
    const float spacing = 5;
    sf::Vector2f input_size(size.x * 0.8f, m_input.getCharacterSize() + m_label.getCharacterSize() + spacing * 3.0f);
    m_input_bg.setFillColor(sf::Color(42, 38, 49));
    m_input_bg.setSize(input_size);
    m_input_bg.move(size.x * 0.1f, size.y * 0.5f - input_size.y * 0.5f);

    sf::Vector2f label_pos(m_input_bg.getGlobalBounds().left, m_input_bg.getGlobalBounds().top);
    m_label.setPosition(label_pos);
    m_label.move(spacing * 1.5f, spacing * 0.5f);

    m_input.setPosition(label_pos);
    m_input.move(spacing * 2.f, spacing + m_label.getCharacterSize());
}

auto TextInput::setInputLabel(std::string label) -> void { m_label.setString(label); }
auto TextInput::setMainLabel(std::string label) -> void { m_main_label.setString(label); }

auto TextInput::draw(sf::RenderTarget &target, sf::RenderStates states) const -> void {
    if (!m_show)
        return;

    target.draw(m_bg);
    target.draw(m_input_bg);
    target.draw(m_main_label);
    target.draw(m_label);
    target.draw(m_input);
}

auto TextInput::update() -> void {
    m_text_animation += m_clock.restart();

    if (m_text_animation >= sf::seconds(0.5f)) {
        m_show_cursor = !m_show_cursor;
        m_text_animation = sf::Time::Zero;
    }

    m_input.setString(m_string + (m_show_cursor ? '|' : ' '));
}

auto TextInput::handleEvent(sf::Event event) -> void {
    if (!m_active)
        return;

    if (event.type == sf::Event::TextEntered) {
        if (std::isalpha(event.text.unicode) && m_string.length() < m_input_max_length) {
            m_string += event.text.unicode;
            if (m_callbacks.edit)
                m_callbacks.edit(m_string);
        }
    }

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::BackSpace) {
            if (!m_string.empty()) {
                m_string.pop_back();
                if (m_callbacks.edit)
                    m_callbacks.edit(m_string);
            }
        } else if (event.key.code == sf::Keyboard::Return) {
            if (m_callbacks.submit)
                m_callbacks.submit(m_string);
            m_string = "";
            this->hide();
        } else if (event.key.code == sf::Keyboard::Escape) {
            m_string = "";
            this->hide();
        }
    }
}
auto TextInput::setSubmitCallback(TextInput::SubmitCallback callback) -> void { m_callbacks.submit = callback; }
auto TextInput::setEditCallback(TextInput::EditCallback callback) -> void { m_callbacks.edit = callback; }

auto TextInput::setInputLimit(size_t input_max_length) -> void { m_input_max_length = input_max_length; }
auto TextInput::setString(std::string str) -> void { m_input.setString(str); }

auto TextInput::visible() const noexcept -> bool { return m_show; }
auto TextInput::activate() noexcept -> void { m_active = true; }
auto TextInput::deactivate() noexcept -> void { m_active = false; }
auto TextInput::show() noexcept -> void {
    m_clock.restart();
    m_show = true;
    m_active = true;
}
auto TextInput::hide() noexcept -> void {
    m_show = false;
    m_active = false;
}

auto TextInput::getBG() -> sf::RectangleShape & { return m_bg; }
auto TextInput::getInputBg() -> sf::RectangleShape & { return m_input_bg; }
auto TextInput::getText() -> sf::Text & { return m_input; }
auto TextInput::getString() -> std::string { return m_string; }