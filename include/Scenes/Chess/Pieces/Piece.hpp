#ifndef PIECE_HPP
#define PIECE_HPP

#include "./../Move.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Chess;
class Piece;

enum class Team {
    WHITE, // Start the game, tradicionally the white pieces
    BLACK  // Goes last in the game, tradicionally the black pieces
};

#define FACTORY_NAME_PIECE_MAKER "createPiece"

using MoveList = std::vector<Move>;

using PieceMaker = Piece *(*)();

class Piece {
  public:
    Piece();
    Piece(Team t_team, Move::BoardPos t_position, const sf::Sprite &t_sprite);
    virtual ~Piece() {}
    virtual auto getMoves(std::function<Piece *(Move::BoardPos)> hasPiece, Move::BoardPos board_size) -> MoveList & { return _move_list_data; }

    auto getOldMoves() -> const MoveList &;
    auto getTeam() -> Team;
    auto getSprite() -> sf::Sprite &;

    Move::BoardPos position;
    std::function<void()> onDie;
    std::function<void(Move::BoardPos)> onMove;
    std::function<void(std::unique_ptr<Piece> &, std::unordered_map<std::string, sf::Sprite> &)> onReachEnd;

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