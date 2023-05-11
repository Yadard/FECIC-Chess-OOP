#ifndef CHESS_HPP
#define CHESS_HPP

#include "Preset.hpp"
#include "Scenes/Chess/Board.hpp"
#include "Scenes/Chess/History.hpp"
#include "Scenes/Chess/KillZone.hpp"
#include "Scenes/Scene.hpp"
#include "Scenes/Victory.hpp"


#include <SFML/Graphics.hpp>
#include <functional>

namespace Scene {
class Chess : public IScene, public Match {
  public:
    Chess(sf::RenderWindow &render, std::function<void()> t_quit, std::function<void(IScene *)> t_change_scene, Preset &preset, bool new_preset = false);

    auto update(sf::RenderWindow &render) -> void override;
    auto draw(sf::RenderWindow &render) -> void override;
    auto handle_input(std::function<void(Scene::IScene *)> change_scene, std::function<void()> quit, sf::Event event) -> void override;

    auto win(Team team) -> void override;
    auto createPiece(std::string name, Team team, sf::Vector2u pos) -> Piece * override;

    // std::array<std::function<void(sf::Event &)>, EVENT_AMOUNT_SFML> _event_handlers{nullptr};

  private:
    auto loadPresetOnBoard(Preset &preset) -> void;
    auto handleMousePress(sf::Event event) -> void;
    auto handleKeyboardPress(sf::Event event) -> void;

    auto executeMove(Move &move) -> Piece *;
    auto clearMoves(const Piece *piece, sf::Vector2u pos) -> void;
    auto highlightMoves(size_t piece_index) -> void;
    auto reachedEnemyField(Move &move) -> bool;
    auto endTurn() -> void;

    Board m_board;
    History m_history;
    Preset m_preset;
    sf::Vector2f m_viewport;
    struct __Killzones {
        KillZone starter_team, last_team;

        __Killzones(KillZone __a, KillZone __b) : starter_team(__a), last_team(__b) {}
    } m_killzones;
    bool m_new_preset = false;
    const Piece *m_selected_piece = nullptr;
    Team m_current_turn = Team::WHITE;
    struct {
        size_t white = 0, black = 0;
    } m_pieces_amount;

    std::unique_ptr<Victory> m_victory;
};
} // namespace Scene

#endif // CHESS_HPP