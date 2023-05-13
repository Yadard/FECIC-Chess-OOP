#ifndef TEXTINPUT_HPP
#define TEXTINPUT_HPP

#include "AssetManager.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <locale>

class TextInput : public sf::Drawable {
  public:
    using SubmitCallback = std::function<void(std::string)>;
    using EditCallback = std::function<void(std::string)>;

    auto draw(sf::RenderTarget &target, sf::RenderStates states) const -> void override;
    auto update() -> void;
    auto handleEvent(sf::Event event) -> void;

    auto setSubmitCallback(SubmitCallback callback) -> void;
    auto setEditCallback(EditCallback callback) -> void;

    auto activate() noexcept -> void;
    auto deactivate() noexcept -> void;
    auto show() noexcept -> void;
    auto hide() noexcept -> void;
    auto visible() const noexcept -> bool;

    auto setInputLimit(size_t input_max_length) -> void;
    auto setDimensions(sf::Vector2f pos, sf::Vector2f size) -> void;
    auto setInputLabel(std::string label) -> void;
    auto setMainLabel(std::string label) -> void;
    auto setString(std::string str) -> void;

    auto getBG() -> sf::RectangleShape &;
    auto getInputBg() -> sf::RectangleShape &;
    auto getText() -> sf::Text &;
    auto getString() -> std::string;

  private:
    sf::Text m_main_label;
    sf::Text m_label;
    sf::Text m_input;
    std::string m_string;
    sf::RectangleShape m_bg;
    sf::RectangleShape m_input_bg;
    sf::Clock m_clock;
    sf::Time m_text_animation;
    size_t m_input_max_length;

    struct {
        SubmitCallback submit;
        EditCallback edit;
    } m_callbacks;

    bool m_show_cursor = true;
    bool m_show = false;
    bool m_active = false;
};

#endif // TEXTINPUT_HPP