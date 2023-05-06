#ifndef BOARD_H
#define BOARD_H

#include "Move.hpp"
#include "Pieces/piece.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

// TODO: Implement a dynamic way of generate a board;

struct Tile : sf::RectangleShape {

    sf::Color original_color;
    std::unique_ptr<Piece> piece = nullptr;
    std::unique_ptr<Move> move = nullptr;

    Tile(sf::Color t_original_color, Piece *t_piece, Move *t_move)
        : original_color(t_original_color), piece(t_piece), move(t_move)

    {}
};

class Board {
  public:
    Board(sf::Vector2f t_position, sf::Vector2f t_viewport, sf::Vector2u t_board_dimensions, sf::Color t_primary_tile = sf::Color(118, 150, 86),
          sf::Color t_secondary_tile = sf::Color(238, 238, 210)) {
        makeBoard(t_position, t_viewport, t_board_dimensions, t_primary_tile, t_secondary_tile);
    }
    Board() = default;
    auto makeBoard(sf::Vector2f t_position, sf::Vector2f t_viewport, sf::Vector2u t_board_dimensions, sf::Color t_primary_tile = sf::Color(118, 150, 86),
                   sf::Color t_secondary_tile = sf::Color(238, 238, 210)) -> void;

    auto getTileFromMousePostion(sf::Vector2f mouse_position) -> Tile *;

    auto executeMove(Move &move) -> std::unique_ptr<Piece> &;
    auto getMoves(Piece *piece) -> MoveList &;

    auto addPiece(std::unique_ptr<Piece> &piece) -> void;

    auto highlightMoves(Piece *piece) -> void;

    auto hasPiece(Move::BoardPos board_position) -> Piece *;
    auto clearHighlightMoves(Piece *moved_piece) -> void;

    auto draw(sf::RenderWindow &render) -> void;

    auto handleMouseClick(sf::FloatRect mouse) -> Tile *;
    auto update(sf::RenderWindow &render) -> void;
    auto resize(sf::Vector2u board_dimensions) -> void;
    auto resize(uint32_t rows, uint32_t cols) -> void;

    auto getBoardDimensions() const -> sf::Vector2u;

  private:
    auto generateBoard(sf::Color primary_color, sf::Color secondary_color) -> void;
    inline auto getTile(size_t row, size_t col) -> Tile &;

    sf::RectangleShape m_borders;
    sf::Vector2f m_position;
    sf::Vector2f m_viewport;
    sf::Vector2u m_board_dimensions;
    sf::Vector2f m_tile_size;
    sf::Color m_primary_tile;
    sf::Color m_secondary_tile;

    std::vector<Tile> m_tiles;
};

#endif // BOARD_H