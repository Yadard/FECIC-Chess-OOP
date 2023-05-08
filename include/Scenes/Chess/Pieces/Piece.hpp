#ifndef PIECE_HPP
#define PIECE_HPP

#include "./../Match.hpp"
#include "./../Move.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
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

using MoveList = std::vector<Move>;

using PieceMaker = Piece *(*)(Team, Move::BoardPos, const sf::Texture &t_texture);

class Piece {
  public:
    Piece(Team t_team, Move::BoardPos t_position, const sf::Texture &t_texture);
    virtual auto getMoves(std::function<Piece *(Move::BoardPos)> hasPiece, Move::BoardPos board_size) -> const MoveList & = 0;

    auto getOldMoves() const -> const MoveList &;
    auto getTeam() const -> Team;
    auto getSprite() const -> const sf::Sprite &;
    auto getSprite() -> sf::Sprite &;
    auto addOldMoves(Move move) -> void;

    Move::BoardPos position;
    std::function<void(Match *)> onDie;
    std::function<void(Match *, Move::BoardPos)> onMove;
    std::function<void(Match *, std::unique_ptr<Piece> &)> onReachEnd;

  protected:
    auto goFowards(Move::BoardPos position, size_t amount = 1) -> Move::BoardPos;
    auto goBackwards(Move::BoardPos position, size_t amount = 1) -> Move::BoardPos;
    auto goLeft(Move::BoardPos position, size_t amount = 1) -> Move::BoardPos;
    auto goRight(Move::BoardPos position, size_t amount = 1) -> Move::BoardPos;

    auto inBounds(Move::BoardPos position, Move::BoardPos board_size) -> bool;
    auto isEnemy(Piece *piece) -> bool;
    auto isAlly(Piece *piece) -> bool;

    sf::Sprite _sprite;
    MoveList _move_list_data;
    Team _team;
};

#endif // PIECE_HPP