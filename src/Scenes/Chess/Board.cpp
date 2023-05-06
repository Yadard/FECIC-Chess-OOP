#include "Scenes/Chess/Board.hpp"

auto Board::makeBoard(sf::Vector2f t_position, sf::Vector2f t_viewport, sf::Vector2u t_board_dimensions, sf::Color t_primary_tile, sf::Color t_secondary_tile)
    -> void {
    m_position = t_position;
    m_viewport = t_viewport;
    m_board_dimensions = t_board_dimensions;
    m_primary_tile = t_primary_tile;
    m_secondary_tile = t_secondary_tile;
    m_tile_size = sf::Vector2f{m_viewport.x / static_cast<float>(m_board_dimensions.x), m_viewport.y / static_cast<float>(m_board_dimensions.y)};

    m_borders.setSize(t_viewport);
    m_borders.setPosition(t_position);
    m_borders.setOutlineColor(sf::Color(64, 47, 29));
    m_borders.setOutlineThickness(5);
    m_tiles.reserve(m_board_dimensions.x * m_board_dimensions.y * 1.5f);
    for (size_t i = 0; i < m_tiles.capacity(); i++)
        m_tiles.emplace_back(sf::Color::White, nullptr, nullptr);

    generateBoard(m_primary_tile, m_secondary_tile);
}

auto Board::getBoardDimensions() const -> sf::Vector2u { return m_board_dimensions; }

auto Board::getTileFromMousePostion(sf::Vector2f mouse_position) -> Tile * {
    mouse_position.x -= m_position.x;
    mouse_position.y -= m_position.y;

    if (mouse_position.y < 0 || mouse_position.y > this->m_viewport.y || mouse_position.x < 0 || mouse_position.x > this->m_viewport.x)
        return nullptr;
    return &getTile(mouse_position.y / m_tile_size.y, mouse_position.x / m_tile_size.x);
}

auto Board::executeMove(Move &move) -> std::unique_ptr<Piece> & {
    std::unique_ptr<Piece> &piece = getTile(move.getMoveDestination().y, move.getMoveDestination().x).piece;
    move.getPiece()->position = move.getMoveDestination();
    piece.swap(getTile(move.getMoveOrigin().y, move.getMoveOrigin().x).piece);
    return piece;
}

auto Board::getMoves(Piece *piece) -> MoveList & { return piece->getMoves(std::bind(&Board::hasPiece, this, std::placeholders::_1), m_board_dimensions); }

auto Board::addPiece(std::unique_ptr<Piece> &piece) -> void {
    sf::Vector2f center(piece->getSprite().getGlobalBounds().width, piece->getSprite().getGlobalBounds().height);
    center.x *= 0.5f;
    center.y *= 0.5f;
    piece->getSprite().setOrigin(center);
    getTile(piece->position.y, piece->position.x).piece = std::move(piece);
}

auto Board::highlightMoves(Piece *piece) -> void {
    auto moves = piece->getMoves(std::bind(&Board::hasPiece, this, std::placeholders::_1), m_board_dimensions);

    if (!moves.empty()) {
        Tile *tile = &getTile(moves[0].getMoveOrigin().y, moves[0].getMoveOrigin().x);
        tile->setFillColor(sf::Color(tile->original_color.r - 50, tile->original_color.g - 50, tile->original_color.b - 50));
        for (auto it = moves.begin(); it != moves.end(); it++) {
            tile = &getTile(it->getMoveDestination().y, it->getMoveDestination().x);
            tile->setFillColor(sf::Color(sf::Color::Green));
            tile->move = std::move(std::make_unique<Move>(*it));
        }
    }
}

auto Board::hasPiece(Move::BoardPos board_position) -> Piece * {
    Tile &tile = getTile(board_position.y, board_position.x);
    return tile.piece.get();
}

auto Board::clearHighlightMoves(Piece *moved_piece) -> void {
    for (auto &coord : moved_piece->getOldMoves()) {
        Tile &tile = getTile(coord.getMoveDestination().y, coord.getMoveDestination().x);
        tile.setFillColor(tile.original_color);
    }
}

auto Board::draw(sf::RenderWindow &render) -> void {
    render.draw(m_borders);
    for (auto row = 0; row < m_board_dimensions.y; row++) {
        for (auto col = 0; col < m_board_dimensions.x; col++) {
            Tile &curr_tile = getTile(row, col);
            render.draw(curr_tile);
            if (curr_tile.piece)
                render.draw(curr_tile.piece->getSprite());
        }
    }
}

auto Board::handleMouseClick(sf::FloatRect mouse) -> Tile * {
    Tile *tile = getTileFromMousePostion({mouse.left, mouse.top});
    return tile;
}

auto Board::update(sf::RenderWindow &render) -> void { this->draw(render); }
auto Board::resize(sf::Vector2u board_dimensions) -> void { resize(board_dimensions.x, board_dimensions.y); }
auto Board::resize(uint32_t cols, uint32_t rows) -> void {
    if (rows * cols > m_tiles.capacity())
        m_tiles.reserve(rows * cols);

    for (size_t i = m_tiles.size(); i < m_tiles.capacity(); i++)
        m_tiles.emplace_back(sf::Color::White, nullptr, nullptr);

    m_board_dimensions.y = rows;
    m_board_dimensions.x = cols;

    generateBoard(m_primary_tile, m_secondary_tile);
}

inline auto Board::getTile(size_t row, size_t col) -> Tile & { return m_tiles[row * m_board_dimensions.x + col]; }

auto Board::generateBoard(sf::Color primary_color, sf::Color secondary_color) -> void {
    m_tile_size = sf::Vector2f{m_viewport.x / static_cast<float>(m_board_dimensions.x), m_viewport.y / static_cast<float>(m_board_dimensions.y)};

    for (size_t row = 0; row < m_board_dimensions.y; row++) {
        for (size_t col = 0; col < m_board_dimensions.x; col++) {
            Tile &curr_tile = getTile(row, col);

            curr_tile.setSize(m_tile_size);
            curr_tile.setPosition(m_position.x + m_tile_size.x * col, m_position.y + m_tile_size.y * row);

            sf::Color tile_color;
            if (col % 2 == 0) {
                if (row % 2 == 0) {
                    tile_color = primary_color;
                } else {
                    tile_color = secondary_color;
                }
            } else {
                if (row % 2 == 0) {
                    tile_color = secondary_color;
                } else {
                    tile_color = primary_color;
                }
            }
            curr_tile.original_color = tile_color;
            curr_tile.setFillColor(tile_color);
        }
    }
}