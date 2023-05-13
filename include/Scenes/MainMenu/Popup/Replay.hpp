#ifndef REPLAYPOPUP_HPP
#define REPLAYPOPUP_HPP

#include "AssetManager.hpp"
#include "Replay.hpp"
#include "Scenes/Button.hpp"
#include "Scenes/Chess/Board.hpp"
#include "Scenes/Scene.hpp"
#include "Scenes/TextInput.hpp"
#include "ThreadDispatcher.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class Arrow : public sf::Drawable {
  public:
    Arrow() : m_shape(sf::Triangles, 9) {}

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override { target.draw(m_shape); }

    auto setDimensions(sf::Vector2f pos, sf::Vector2f size, bool point_to_right = true) -> void {
        m_pos = pos;
        m_size = size;

        size_t i = 0;
        m_shape[i].position = pos;
        m_shape[i++].color = sf::Color(19, 55, 116);

        sf::Vector2f rect_size(m_size.x * 0.1f, m_size.y);
        pos.y += rect_size.y;
        m_shape[i].position = pos;
        m_shape[i++].color = sf::Color(19, 55, 116);

        pos.x += rect_size.x;
        m_shape[i].position = pos;
        m_shape[i++].color = sf::Color(19, 55, 116);

        m_shape[i].position = pos;
        m_shape[i++].color = sf::Color(19, 55, 116);

        pos.y -= rect_size.y;
        m_shape[i].position = pos;
        m_shape[i++].color = sf::Color(19, 55, 116);

        pos.x -= rect_size.x;
        m_shape[i].position = pos;
        m_shape[i++].color = sf::Color(19, 55, 116);

        sf::Vector2f trig_size(m_size.x - rect_size.x, m_size.y);

        if (point_to_right) {
            pos.x += rect_size.x;
            m_shape[i].position = pos;
            m_shape[i++].color = sf::Color(19, 55, 116);

            pos.x += trig_size.x;
            pos.y += trig_size.y * 0.5f;
            m_shape[i].position = pos;
            m_shape[i++].color = sf::Color(19, 55, 116);

            pos.x -= trig_size.x;
            pos.y += trig_size.y * 0.5f;
            m_shape[i].position = pos;
            m_shape[i++].color = sf::Color(19, 55, 116);
        } else {
            m_shape[i].position = pos;
            m_shape[i++].color = sf::Color(19, 55, 116);

            pos.x -= trig_size.x;
            pos.y += trig_size.y * 0.5f;
            m_shape[i].position = pos;
            m_shape[i++].color = sf::Color(19, 55, 116);

            pos.x += trig_size.x;
            pos.y += trig_size.y * 0.5f;
            m_shape[i].position = pos;
            m_shape[i++].color = sf::Color(19, 55, 116);
        }
    }
    auto getPosition() -> sf::Vector2f { return m_pos; }
    auto getSize() -> sf::Vector2f { return m_size; }
    auto getHitbox() -> sf::FloatRect { return m_shape.getBounds(); }

  private:
    sf::VertexArray m_shape;
    sf::Vector2f m_pos;
    sf::Vector2f m_size;
};

class ReplayPopup : public sf::Drawable {
  public:
    ReplayPopup(sf::RenderWindow &t_render, std::function<void(const Replay &)> t_replay_player);

    auto hide() -> void;
    auto show() -> void;
    auto isVisible() const -> bool;

    auto draw(sf::RenderTarget &target, sf::RenderStates states) const -> void override;
    auto handleInput(sf::Event event) -> bool;
    auto setBackground(sf::RectangleShape &bg) -> void;

  private:
    struct Entry {
        Replay replay;
        std::string name;

        Entry(Replay replay, const std::string &name) : replay(replay), name(name) {}
    };

    auto loadReplays() -> void;
    auto applyReplay(const Entry &entry) -> void;

  private:
    bool m_show = false;

    sf::RectangleShape m_background;
    sf::RectangleShape m_presets_bg;
    std::vector<Entry> m_replays;
    std::vector<Entry>::reverse_iterator m_current_replay;
    std::function<void(const Replay &)> m_replay_player;

    Arrow m_left_arrow, m_right_arrow;
    TextInput m_replay_name;
    Board m_preview;
};

#endif // REPLAYPOPUP_HPP