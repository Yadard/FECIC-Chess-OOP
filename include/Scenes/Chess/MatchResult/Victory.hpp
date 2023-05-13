#ifndef VICTORY_HPP
#define VICTORY_HPP

#include "Scenes/Chess/MatchResult/MatchResult.hpp"

class Victory : public MatchResult {
  public:
    Victory(sf::Vector2f viewport, std::function<void()> t_mainmenu, std::function<void()> t_play_again, Preset preset, bool new_board, Replay replay,
            Team winner);

    auto draw(sf::RenderTarget &target, sf::RenderStates states) const -> void override;
    auto update() -> void override;
    auto handleInput(sf::Event) -> void override;

  private:
    sf::Text m_label;
};

#endif // VICTORY_HPP