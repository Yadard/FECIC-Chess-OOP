#ifndef PIECE_HPP
#define PIECE_HPP

#include "Scenes/Chess/Match.hpp"
#include "Scenes/Chess/Move.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>
#include <string>
#include <vector>

class Piece;

#define FACTORY_NAME_PIECE_MAKER "createPiece"

#ifdef _WIN32
#define PIECE_API_EXPORT __declspec(dllexport)
#define PIECE_API_IMPORT __declspec(dllimport)
#else
#define SFML_API_EXPORT __attribute__((__visibility__("default")))
#define SFML_API_IMPORT __attribute__((__visibility__("default")))
#endif

using PieceMaker = Piece *(*)(Team, Move::BoardPos, const sf::Texture &t_texture);

struct PieceInfo {
    std::string name;
    sf::Vector2u board_pos;
    Team team;
};

class Piece {
  public:
    Piece(Team t_team, Move::BoardPos t_position, const sf::Texture &t_texture);
    virtual auto getMoves(Match *match) -> const std::vector<Move> & = 0;
    virtual auto doSpecialMove(Match *match) -> void = 0;

    auto getOldMoves() const -> const std::vector<Move> &;
    auto getTeam() const -> Team;
    auto getSprite() const -> const sf::Sprite &;
    auto getSprite() -> sf::Sprite &;
    auto addOldMoves(Move move) -> void;

    Move::BoardPos position;
    std::function<void(Match *)> onDie;
    std::function<void(Match *, Move::BoardPos)> onMove;
    std::function<void(Match *, std::unique_ptr<Piece> &)> onReachEnd;

  protected:
    auto goForwards(Move::BoardPos position, size_t amount = 1) -> Move::BoardPos;
    auto goBackwards(Move::BoardPos position, size_t amount = 1) -> Move::BoardPos;
    auto goLeft(Move::BoardPos position, size_t amount = 1) -> Move::BoardPos;
    auto goRight(Move::BoardPos position, size_t amount = 1) -> Move::BoardPos;

    auto inBounds(Move::BoardPos position, Move::BoardPos board_size) -> bool;
    auto isEnemy(Piece *piece) -> bool;
    auto isAlly(Piece *piece) -> bool;

    sf::Sprite m_sprite;
    std::vector<Move> m_move_list_data;
    Team m_team;
};

#endif // PIECE_HPP