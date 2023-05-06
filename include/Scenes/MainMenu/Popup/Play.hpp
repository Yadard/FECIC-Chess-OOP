#ifndef PLAY_HPP
#define PLAY_HPP

#include "./../../../AssetManager.hpp"
#include "./../../Chess/Board.hpp"
#include "./../../Scene.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class TextInput {};

class PlayPopup {
  public:
    class Button {
      public:
        Button(std::string label_text);

        auto clicked(sf::Event event, sf::FloatRect mouse) -> bool;
        auto setPosition(sf::Vector2f position) -> void;

        auto draw(sf::RenderWindow &render) -> void;

      private:
        sf::RectangleShape m_background;
        sf::Text m_label;
    };

    struct PresetEntry {
        const Preset &preset;
        sf::Text btn;
        sf::RectangleShape line;

        PresetEntry(const Preset &t_preset, std::string str) : preset(t_preset) {
            btn.setString(str);
            btn.setFont(AssetManager::GetInstance().getFont("MainMenu.Button"));
            btn.setFillColor(sf::Color::Black);
            btn.setCharacterSize(20);

        }

        auto draw(sf::RenderWindow &render) -> void {
            render.draw(btn);
            render.draw(line);
        }
        auto clicked(sf::FloatRect mouse) -> bool { return btn.getGlobalBounds().intersects(mouse); }
    };

    PlayPopup(sf::RenderWindow &render);

    auto hide() -> void;
    auto show() -> void;
    auto isVisible() -> bool;

    auto draw(sf::RenderWindow &render) -> void;
    auto handleInput(std::function<void(Scene::IScene *)> change_scene, sf::Event event, sf::FloatRect mouse) -> bool;

  private:
    auto loadPresets() -> void;
    auto applyPreset() -> void;

    bool m_show = false;

    sf::RectangleShape m_background;
    sf::RectangleShape m_presets_bg;
    std::vector<PresetEntry> m_presets;
    // TextInput m_row, m_col;
    // Board m_preview;

    Button m_play_btn, m_close_btn;
};

#endif // PLAY_HPP