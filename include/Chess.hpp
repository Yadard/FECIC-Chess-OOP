#ifndef CHESS_HPP
#define CHESS_HPP

#include "Board.hpp"
#include "History.hpp"
#include "KillZone.hpp"
#include <SFML/Graphics.hpp>
#include <array>
#include <functional>

#define EVENT_AMOUNT_SFML 23

class Chess {
  public:
    Chess();
    auto addPiece(std::unique_ptr<Piece> piece) -> void;
    auto run() -> void;
    auto win(Team winner) -> void;

  private:
    auto endTurn() -> void;
    auto onMouseButtonPress(sf::Event &event) -> void;
    auto onWindowClose(sf::Event &event) -> void;

    std::array<std::function<void(sf::Event &)>, EVENT_AMOUNT_SFML> _event_handlers{nullptr};

    sf::RenderWindow _render;
    Board<8, 8> _board;
    History _history;
    struct __Killzones {
        KillZone starter_team, last_team;

        __Killzones(KillZone __a, KillZone __b) : starter_team(__a), last_team(__b) {}
    } _killzones;
    bool _drawn = false;
    Piece *selected_piece = nullptr;
    Team current_turn = Team::STARTER;
    struct {
        size_t starter = 0, last = 0;
    } pieces_amount;
};

#endif // CHESS_HPP