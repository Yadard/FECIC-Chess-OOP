#ifndef CHESS_HPP
#define CHESS_HPP

#include "Board.hpp"
#include "History.hpp"
#include "KillZone.hpp"
#include <SFML/Graphics.hpp>
#include <array>
#include <functional>
#include <unordered_map>

#define EVENT_AMOUNT_SFML 23

class Chess {
  public:
    Chess();
    auto addPiece(std::unique_ptr<Piece> piece) -> void;
    auto registerSprite(std::string key, sf::Sprite &sprite) -> void;
    auto getSprite(std::string key) -> const sf::Sprite &;
    auto run() -> void;
    auto win(Team winner) -> void;

  private:
    auto endTurn() -> void;
    auto onMouseButtonPress(sf::Event &event) -> void;
    auto onWindowClose(sf::Event &event) -> void;

    std::array<std::function<void(sf::Event &)>, EVENT_AMOUNT_SFML> _event_handlers{nullptr};

    sf::RenderWindow _render;
    Board _board;
    History _history;
    struct __Killzones {
        KillZone starter_team, last_team;

        __Killzones(KillZone __a, KillZone __b) : starter_team(__a), last_team(__b) {}
    } _killzones;
    bool _drawn = false;
    std::unique_ptr<Piece> *selected_piece = nullptr;
    Team current_turn = Team::WHITE;
    struct {
        size_t starter = 0, last = 0;
    } pieces_amount;

    std::unordered_map<std::string, sf::Sprite> sprites;
};

#endif // CHESS_HPP