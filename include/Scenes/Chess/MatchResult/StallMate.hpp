#ifndef STALLMATE_HPP
#define STALLMATE_HPP

#include "Scenes/Chess/MatchResult/MatchResult.hpp"

class StallMate : public MatchResult {
  public:
    StallMate(sf::RenderWindow &render, std::function<void(Scene::IScene *)> t_change_scene, Preset preset, bool new_board, Replay replay);

    auto draw(sf::RenderTarget &target, sf::RenderStates states) const -> void override;
    auto handleInput(sf::Event) -> void override;

  private:
    /* data */
};

#endif // STALLMATE_HPP