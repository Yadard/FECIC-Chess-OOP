#ifndef BOARD_H
#define BOARD_H

#include "Move.hpp"
#include "Pieces/piece.hpp"
#include <SFML/Graphics.hpp>
#include <array>
#include <memory>

struct Tile : sf::RectangleShape {

    sf::Color original_color;
    std::unique_ptr<Piece> piece = nullptr;
    std::unique_ptr<Move> move = nullptr;
};

template <size_t ROW_AMOUNT, size_t COL_AMOUNT>
class Board : sf::RectangleShape {
  public:
    Board(sf::Vector2f t_position, sf::Vector2u t_size, sf::Color t_starter_tile = sf::Color(118, 150, 86), sf::Color t_last_tile = sf::Color(238, 238, 210))
        : _position(t_position), _size(t_size), _starter_tile(t_starter_tile), _last_tile(t_last_tile) {
        _tile_size = sf::Vector2f{static_cast<float>(_size.x / ROW_AMOUNT), static_cast<float>(_size.y / COL_AMOUNT)};
        this->setPosition(t_position);
        for (size_t i = 0; i < tiles.size(); i++) {
            for (size_t j = 0; j < tiles[i].size(); j++) {
                tiles[i][j].setPosition({static_cast<float>(_position.x + j * _tile_size.x), static_cast<float>(_position.y + i * _tile_size.y)});
                tiles[i][j].setSize(_tile_size);
                if (((i * (COL_AMOUNT - 1)) + j) % 2 == 0) {
                    tiles[i][j].setFillColor(_starter_tile);
                    tiles[i][j].original_color = _starter_tile;
                } else {
                    tiles[i][j].setFillColor(_last_tile);
                    tiles[i][j].original_color = _last_tile;
                }
            }
        }
    }

    auto getTileFromMousePostion(sf::Vector2i mouse_position) -> Tile * {
        mouse_position.x -= this->getPosition().x;
        mouse_position.y -= this->getPosition().y;

        if (mouse_position.y < 0 || mouse_position.y > this->_size.y || mouse_position.x < 0 || mouse_position.x > this->_size.x)
            return nullptr;
        return &tiles[mouse_position.y / _tile_size.y][mouse_position.x / _tile_size.x];
    }

    auto executeMove(Move &move) -> std::unique_ptr<Piece> & {
        std::unique_ptr<Piece> &piece = tiles[move.getMoveDestination().y][move.getMoveDestination().x].piece;
        move.getPiece()->position = move.getMoveDestination();
        tiles[move.getMoveDestination().y][move.getMoveDestination().x].piece.swap(tiles[move.getMoveOrigin().y][move.getMoveOrigin().x].piece);
        return piece;
    }

    auto getMoves(Piece *piece) -> MoveList & {
        return piece->getMoves(std::bind(&Board<ROW_AMOUNT, COL_AMOUNT>::hasPiece, this, std::placeholders::_1), {COL_AMOUNT, ROW_AMOUNT});
    }

    auto addPiece(std::unique_ptr<Piece> &piece) -> void { tiles[piece->position.y][piece->position.x].piece = std::move(piece); }

    auto highlightMoves(Piece *piece) -> void {
        auto moves = piece->getMoves(std::bind(&Board<ROW_AMOUNT, COL_AMOUNT>::hasPiece, this, std::placeholders::_1), {COL_AMOUNT, ROW_AMOUNT});

        if (!moves.empty()) {
            Tile *tile = &tiles[moves[0].getMoveOrigin().y][moves[0].getMoveOrigin().x];
            tile->setFillColor(sf::Color(tile->original_color.r - 50, tile->original_color.g - 50, tile->original_color.b - 50));
            for (auto it = moves.begin(); it != moves.end(); it++) {
                tile = &tiles[it->getMoveDestination().y][it->getMoveDestination().x];
                tile->setFillColor(sf::Color(sf::Color::Green));
                tile->move = std::move(std::make_unique<Move>(*it));
            }
        }
    }

    auto hasPiece(Move::BoardPos board_position) -> Piece * {
        Tile &tile = tiles[board_position.y][board_position.x];
        return tile.piece.get();
    }

    auto clearHighlightMoves(Piece *moved_piece) -> void {
        for (auto &coord : moved_piece->getOldMoves()) {
            Tile *tile = getTile(coord.getMoveDestination().y, coord.getMoveDestination().x);
            tile->setFillColor(tile->original_color);
        }
    }

    auto draw(sf::RenderWindow &render) -> void {
        for (auto i = 0; i < tiles.size(); i++) {
            for (auto j = 0; j < tiles[i].size(); j++) {
                render.draw(tiles[i][j]);
                if (tiles[i][j].piece) {
                    auto center = tiles[i][j].getPosition();
                    center.x += (_tile_size.x - 70) / 2;
                    center.y += (_tile_size.y - 70) / 2;
                    tiles[i][j].piece->getSprite().setPosition(center);
                    render.draw(tiles[i][j].piece->getSprite());
                }
            }
        }
    }

    auto update(sf::RenderWindow &render) -> void { this->draw(render); }

  private:
    inline auto getTile(size_t row, size_t col) -> Tile * { return &tiles[row][col]; }

    sf::Vector2u _position;
    sf::Vector2u _size;
    sf::Vector2f _tile_size;
    sf::Color _starter_tile;
    sf::Color _last_tile;
    std::array<std::array<Tile, COL_AMOUNT>, ROW_AMOUNT> tiles;
};

#endif // BOARD_H