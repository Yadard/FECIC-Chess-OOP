#ifndef PLAY_HPP
#define PLAY_HPP

#include "AssetManager.hpp"
#include "Scenes/Button.hpp"
#include "Scenes/Chess/Board.hpp"
#include "Scenes/Scene.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class PlayPopup {
  public:
    PlayPopup(sf::RenderWindow &t_render, std::function<void()> t_quit, std::function<void(Scene::IScene *)> t_change_scene, bool &ugly_fix);

    auto hide() -> void;
    auto show() -> void;
    auto isVisible() -> bool;

    auto draw(sf::RenderWindow &render) -> void;
    auto handleInput(std::function<void(Scene::IScene *)> change_scene, sf::Event event, sf::FloatRect mouse) -> bool;

  private:
    auto loadPresets() -> void;
    auto applyPreset() -> void;
    auto setupButtons(Button &btn, sf::Vector2f pos) -> void;

  public:
    class __Btton {
      public:
        __Btton(std::string label_text);

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

        sf::RectangleShape background;
        sf::RectangleShape line;

        PresetEntry(const Preset &t_preset, std::string str);

        auto draw(sf::RenderWindow &render) -> void;
        auto clicked(sf::FloatRect mouse) -> bool;
    };
    class PieceSelector : public sf::Drawable {
      public:
        struct Entry : sf::Drawable {
            sf::Sprite sprite;
            sf::RectangleShape hitbox;
            std::string piece_name;

            Entry(std::string piece, const sf::Texture &texture, sf::Vector2f size);
            auto setPosition(sf::Vector2f pos) -> void;
            auto setColor(sf::Color origin_tile_color) -> void;
            auto draw(sf::RenderTarget &target, sf::RenderStates states) const -> void override;
        };

        PieceSelector();

        auto handleMouseLeft(std::function<void(std::string, Team, Tile *)> add_piece, sf::FloatRect mouse) -> bool;
        auto handleMouseRight(std::function<void(std::string, Team, Tile *)> add_piece, sf::FloatRect mouse) -> bool;

        auto setSize(sf::Vector2f board_size) -> void;
        auto show(sf::Vector2f pos, Tile &tile, sf::Color origin_tile_color) -> void;
        auto hide() -> void;
        auto visible() -> bool;

      private:
        auto draw(sf::RenderTarget &target, sf::RenderStates states) const -> void override;

        auto generateShape(sf::Vector2f pos, sf::Vector2f rect, sf::Vector2f trig, sf::Color color) -> void;

        bool m_show = false;
        Tile *m_tile;
        size_t m_amount_of_rows;
        std::vector<Entry> m_white_sprites;
        std::vector<Entry> m_black_sprites;
        sf::Vector2f m_entry_size;
        sf::Color m_tile_color;
        sf::Vector2f m_rect;
        sf::Vector2f m_trig;
        sf::VertexArray m_shape;
        sf::Vector2f m_position;
    };

    Preset m_current_preset;
    PresetEntry *m_selected_preset = nullptr;

  private:
    bool m_show = false;

    PieceSelector m_piece_selector;
    sf::RectangleShape m_background;
    sf::RectangleShape m_presets_bg;
    std::vector<PresetEntry> m_presets;

    // TextInput m_row, m_col;
    Board m_preview;

    Button m_play_btn, m_close_btn;
};

#endif // PLAY_HPP