#ifndef REPLAYPLAYER_HPP
#define REPLAYPLAYER_HPP

#include "Replay.hpp"
#include "Scenes/Chess/Board.hpp"
#include "Scenes/Chess/History.hpp"
#include "Scenes/Chess/KillZone.hpp"
#include "Scenes/MainMenu/MainMenu.hpp"
#include "Scenes/Scene.hpp"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <functional>

namespace Scene {
class ReplayPlayer : public IScene, public Match {
  public:
    ReplayPlayer(sf::RenderWindow &render, std::function<void()> t_quit, std::function<void(IScene *)> t_change_scene, const Replay &replay);

    auto update(sf::RenderWindow &render) -> void override;
    auto draw(sf::RenderWindow &render) -> void override;
    auto handle_input(std::function<void(Scene::IScene *)> change_scene, std::function<void()> quit, sf::Event event) -> void override;

    auto win(Team team) -> void override;
    auto createPiece(std::string name, Team team, sf::Vector2u pos) -> Piece * override;
    auto addPiece(Piece *, sf::Vector2u) -> void override;
    auto hasPiece(sf::Vector2u) const -> Piece * override;
    auto getBoardSize() const -> sf::Vector2u override;
    auto endTurn() -> void override;

  private:
    auto loadBoard(const Replay &replay) -> void;
    auto handleMousePress(sf::Event event) -> void;
    auto handleKeyboardPress(sf::Event event) -> void;

    auto executeMove(const Move &move) -> Piece *;
    auto clearMoves(const Piece *piece, sf::Vector2u pos) -> void;
    auto highlightMoves(Piece *piece) -> void;
    auto reachedEnemyField(const Move &move) -> bool;

    Board m_board;
    History m_history;
    Replay m_replay;
    sf::Vector2f m_viewport;
    struct __Killzones {
        KillZone starter_team, last_team;

        __Killzones(KillZone __a, KillZone __b) : starter_team(__a), last_team(__b) {}
    } m_killzones;
    Team m_current_turn = Team::WHITE;
    struct {
        size_t white = 0, black = 0;
    } m_pieces_amount;

    const Piece *m_selected_piece;
    std::vector<Move>::const_iterator m_current_move;
    sf::Sound m_move_sfx;
    sf::Clock m_clock;
    sf::Time m_time;
};

} // namespace Scene

#endif // REPLAYPLAYER_HPP