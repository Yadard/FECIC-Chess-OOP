#ifndef BOARD_H
#define BOARD_H

#include "Scenes/Chess/Move.hpp"
#include "Scenes/Chess/Pieces/Piece.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

// TODO: Implement a dynamic way of generate a board;

class Tile : public sf::RectangleShape {
  public:
    inline static const size_t INVALID_INDEX = std::string::npos;

    Tile(sf::Color t_original_color, Move *t_move)
        : original_color(t_original_color), move(t_move), piece(nullptr)

    {}

    sf::Color original_color;
    std::unique_ptr<Move> move;
    sf::Vector2u board_pos;
    std::unique_ptr<Piece> piece;
};

class Board : public sf::Drawable {
  public:
    Board(sf::Vector2f t_position, sf::Vector2f t_viewport, sf::Vector2u t_board_dimensions, sf::Color t_primary_tile = sf::Color(118, 150, 86),
          sf::Color t_secondary_tile = sf::Color(238, 238, 210)) {
        makeBoard(t_position, t_viewport, t_board_dimensions, t_primary_tile, t_secondary_tile);
    }
    Board() = default;

    auto makeBoard(sf::Vector2f t_position, sf::Vector2f t_viewport, sf::Vector2u t_board_dimensions, sf::Color t_primary_tile = sf::Color(118, 150, 86),
                   sf::Color t_secondary_tile = sf::Color(238, 238, 210)) -> void;

    auto getTileFromMousePostion(sf::Vector2f mouse_position) -> Tile *;
    auto getBoardPosFromMousePostion(sf::Vector2f mouse_position) -> sf::Vector2u;

    auto executeMove(const Move &move) -> std::unique_ptr<Piece> &;
    auto getMoves(Match *match, Piece *piece) -> std::vector<Move>;

    auto addPiece(std::unique_ptr<Piece> &piece, sf::Vector2u pos) -> void;
    auto addPiece(Piece *piece, sf::Vector2u pos) -> void;
    auto removePiece(sf::Vector2u pos) -> void;
    auto clearBoard() -> void;

    auto highlightMoves(Match *match, Piece *piece) -> void;
    auto hasPiece(Move::BoardPos board_position) -> Piece *;
    auto clearMoves(const Piece *moved_piece) -> void;

    auto draw(sf::RenderTarget &target, sf::RenderStates states) const -> void override;
    auto handleMouseClick(sf::FloatRect mouse) -> Tile *;

    auto resize(sf::Vector2u board_dimensions) -> void;
    auto resize(uint32_t rows, uint32_t cols) -> void;

    auto getBoardDimensions() const -> sf::Vector2u;
    auto isMouseInBounds(sf::Vector2f mouse) -> bool;

    auto getTile(size_t row, size_t col, size_t row_size = 0) -> Tile &;
    auto getTile(sf::Vector2u pos, size_t row_size = 0) -> Tile &;
    auto getTile(size_t row, size_t col, size_t row_size = 0) const -> const Tile &;
    auto getTile(sf::Vector2u pos, size_t row_size = 0) const -> const Tile &;

  private:
    auto generateBoard(sf::Color primary_color, sf::Color secondary_color, bool changed_x = false) -> void;

    bool m_need_update = true;
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