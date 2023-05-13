#ifndef MATCHRESULT_HPP
#define MATCHRESULT_HPP

#include "AssetManager.hpp"
#include "Replay.hpp"
#include "Scenes/Button.hpp"
#include "Scenes/MainMenu/MainMenu.hpp"
#include "Scenes/Scene.hpp"
#include "Scenes/TextInput.hpp"
#include "ThreadDispatcher.hpp"
#include <SFML/Graphics.hpp>

class MatchResult : public sf::Drawable {
  public:
    MatchResult(sf::Vector2f viewport, std::function<void()> t_mainmenu, std::function<void()> t_play_again, Preset preset, bool new_board, Replay replay);

    virtual auto draw(sf::RenderTarget &target, sf::RenderStates states) const -> void = 0;
    virtual auto update() -> void = 0;
    virtual auto handleInput(sf::Event) -> void = 0;

    std::function<void()> mainmenu;
    std::function<void()> play_again;
    Preset preset;
    Replay replay;

  protected:
    auto setupButtons(sf::Vector2f pos, sf::Vector2f size, float spacing, bool show_save_board) -> void;

    struct {
        Button play_again;
        Button save_replay;
        Button save_board;
        Button mainmenu;
    } m_btns;

    sf::RectangleShape m_bg;
    TextInput m_input;
    std::unique_ptr<sf::Thread> m_saving_preset;
    std::unique_ptr<sf::Thread> m_saving_replay;
    bool m_new_board;

  private:
    auto setupButton(Button &btn, sf::Vector2f size, sf::Vector2f pos) -> void;
};

#endif // MATCHRESULT_HPP