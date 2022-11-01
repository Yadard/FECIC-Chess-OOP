#ifndef PIECE_HPP
#define PIECE_HPP

#include "./../Move.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

#ifndef MAX_MOVES_AMOUNT
#define MAX_MOVES_AMOUNT 64 // max is a 8x8 square full of moves
#endif

class Chess;

using MoveList = std::vector<Move>;

enum class Team {
    STARTER, // Start the game, tradicionally the white pieces
    LATTER   // Goes last in the game, tradicionally the black pieces
};

class Piece {
  public:
    Piece(Team t_team, Move::BoardPos t_position, sf::Sprite &t_sprite);
    virtual ~Piece() {}
    virtual auto getMoves(std::function<Piece *(Move::BoardPos)> hasPiece, Move::BoardPos board_size) -> MoveList & { return _move_list_data; }

    auto getTeam() -> Team;
    auto getSprite() -> sf::Sprite &;

    Move::BoardPos position;
    std::function<void()> onDie;
    std::function<void(Move::BoardPos)> onMove;

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