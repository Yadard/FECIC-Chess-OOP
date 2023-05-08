#include "Scenes/Chess/Board.hpp"

static sf::Vector2f resizeSprite(sf::Sprite &sprite, const sf::Vector2f &size, bool fit_to_height = true) {
    const sf::Texture *texture = sprite.getTexture();
    sf::Vector2u textureSize = texture->getSize();
    float textureAspectRatio = static_cast<float>(textureSize.x) / textureSize.y;
    float targetAspectRatio = size.x / size.y;

    sf::Vector2f scale;
    if (fit_to_height) {
        // Fit to height
        scale.y = size.y / textureSize.y;
        scale.x = scale.y;
    } else {
        // Fit to width
        scale.x = size.x / textureSize.x;
        scale.y = scale.x;
    }

    sprite.setScale(scale);
    sprite.setPosition(size / 2.f);

    scale.x *= textureSize.x;
    scale.y *= textureSize.y;
    return scale;
}

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
        m_tiles.emplace_back(sf::Color::White, nullptr);

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

auto Board::getBoardPosFromMousePostion(sf::Vector2f mouse_position) -> sf::Vector2u {
    mouse_position.x -= m_position.x;
    mouse_position.y -= m_position.y;

    if (mouse_position.y < 0 || mouse_position.y > this->m_viewport.y || mouse_position.x < 0 || mouse_position.x > this->m_viewport.x)
        return {0, 0};
    return {mouse_position.x / m_tile_size.x, mouse_position.y / m_tile_size.y};
}

auto Board::executeMove(Move &move) -> std::unique_ptr<Piece> & {
    Tile &tile_dest = getTile(move.getMoveDestination().y, move.getMoveDestination().x);
    Tile &tile_orig = getTile(move.getMoveOrigin().y, move.getMoveOrigin().x);
    Piece *new_bystander = nullptr;
    Piece *new_mover = m_pieces[tile_orig.getPiece()].get();
    size_t offset = 0;
    if (tile_dest.getPiece() != Tile::INVALID_INDEX) {
        new_bystander = m_pieces[tile_dest.getPiece()].get();
        if (tile_orig.getPiece() > tile_dest.getPiece()) {
            offset = -1;
        }
    }

    move.getPiece()->position = move.getMoveDestination();

    auto pos = tile_dest.getPosition();
    auto size = new_mover->getSprite().getTexture()->getSize();
    size.x *= new_mover->getSprite().getScale().x;
    size.y *= new_mover->getSprite().getScale().y;
    pos.x += m_tile_size.x * 0.5f - size.x * 0.5f;
    pos.y += m_tile_size.y * 0.5f - size.y * 0.5f;
    new_mover->getSprite().setPosition(pos);
    new_mover->position = move.getMoveDestination();
    if (new_bystander) {
        auto it = std::next(m_pieces.begin(), tile_dest.getPiece());
        it->reset(nullptr);
        m_pieces.erase(it);
    }
    tile_dest.setPiece(tile_orig.getPiece() + offset);
    tile_orig.setPiece(Tile::INVALID_INDEX);
    return m_pieces[tile_dest.getPiece()];
}

auto Board::getMoves(Piece *piece) -> MoveList {
    MoveList moves;
    auto isValid = [this](Move &move) {
        auto inBound = [&](sf::Vector2u vec) { return vec.x < 0 || vec.x > this->m_board_dimensions.x || vec.y < 0 || vec.y > this->m_board_dimensions.y; };
        return move.getPiece() == nullptr || inBound(move.getMoveDestination()) || inBound(move.getMoveOrigin());
    };
    for (auto move : piece->getMoves(std::bind(&Board::hasPiece, this, std::placeholders::_1), m_board_dimensions)) {
        if (isValid(move))
            moves.push_back(move);
    }
    return moves;
}

auto Board::addPiece(std::unique_ptr<Piece> &piece, sf::Vector2u pos) -> void { addPiece(piece.release(), pos); }
auto Board::addPiece(Piece *piece, sf::Vector2u pos) -> void {
    sf::Vector2f size(m_tile_size.x * 0.9f, m_tile_size.y * 0.9f);
    sf::Sprite &sprite = piece->getSprite();
    size = resizeSprite(sprite, size);

    pos.x = std::min(pos.x, m_board_dimensions.x);
    pos.y = std::min(pos.y, m_board_dimensions.y);

    float x_factor = pos.x;
    float y_factor = pos.y;

    Tile &tile = getTile(pos.y, pos.x);
    sf::Vector2f position = tile.getPosition();
    position.x += m_tile_size.x * 0.5f - size.x * 0.5f;
    position.y += m_tile_size.y * 0.5f - size.y * 0.5f;
    sprite.setPosition(position);
    size_t index = m_pieces.size();
    m_pieces.emplace_back(piece);
    tile.setPiece(index);
}

auto Board::removePiece(sf::Vector2u pos) -> void {
    if (getTile(pos.y, pos.x).getPiece() == Tile::INVALID_INDEX)
        return;
    auto erase_it = m_pieces.begin() + getTile(pos.y, pos.x).getPiece();
    for (auto it = erase_it; it != m_pieces.end(); it++) {
        Tile &tile = getTile(it->get()->position.y, it->get()->position.x);
        tile.setPiece(tile.getPiece() - 1);
    }

    m_pieces.erase(erase_it);
    getTile(pos.y, pos.x).setPiece(Tile::INVALID_INDEX);
}

auto Board::highlightMoves(size_t piece_index) -> void {
    Piece *piece = m_pieces[piece_index].get();
    auto &moves = piece->getMoves(std::bind(&Board::hasPiece, this, std::placeholders::_1), m_board_dimensions);

    if (!moves.empty()) {
        Tile *tile = &getTile(moves[0].getMoveOrigin().y, moves[0].getMoveOrigin().x);
        tile->setFillColor(sf::Color(tile->original_color.r - 50, tile->original_color.g - 50, tile->original_color.b - 50));
        tile->setOutlineThickness(-5);
        tile->setOutlineColor(sf::Color::Red);
        for (auto it = moves.begin(); it != moves.end(); it++) {
            tile = &getTile(it->getMoveDestination().y, it->getMoveDestination().x);
            tile->setFillColor(sf::Color(sf::Color::Green));
            tile->move.reset(new Move(*it));
        }
    }
}

auto Board::hasPiece(Move::BoardPos board_position) -> Piece * {
    Tile &tile = getTile(board_position.y, board_position.x);
    if (tile.getPiece() == tile.INVALID_INDEX)
        return nullptr;
    return m_pieces[tile.getPiece()].get();
}

auto Board::clearMoves(const Piece *moved_piece) -> void {
    for (auto &coord : moved_piece->getOldMoves()) {
        Tile &tile = getTile(coord.getMoveDestination().y, coord.getMoveDestination().x);
        tile.setFillColor(tile.original_color);
        tile.move.reset(nullptr);
    }
}

auto Board::draw(sf::RenderWindow &render) -> void {
    render.draw(m_borders);
    for (auto row = 0; row < m_board_dimensions.y; row++) {
        for (auto col = 0; col < m_board_dimensions.x; col++) {
            Tile &curr_tile = getTile(row, col);
            render.draw(curr_tile);
            if (curr_tile.getPiece() != curr_tile.INVALID_INDEX)
                render.draw(m_pieces[curr_tile.getPiece()]->getSprite());
        }
    }
}

auto Board::handleMouseClick(sf::FloatRect mouse) -> Tile * {
    Tile *tile = getTileFromMousePostion({mouse.left, mouse.top});
    return tile;
}

#include <iostream>
#include <utility>

auto Board::update(sf::RenderWindow &render) -> void { this->draw(render); }
auto Board::resize(sf::Vector2u board_dimensions) -> void { resize(board_dimensions.x, board_dimensions.y); }
auto Board::resize(uint32_t cols, uint32_t rows) -> void {
    if (cols == 0 || rows == 0)
        return;

    if (rows * cols > m_tiles.capacity())
        m_tiles.reserve(rows * cols);

    for (size_t i = m_tiles.size(); i < m_tiles.capacity(); i++)
        m_tiles.emplace_back(sf::Color::White, nullptr);

    for (size_t i = 0; i < m_pieces.size(); i++) {
        Tile &old_tile = getTile(m_pieces[i]->position.y, m_pieces[i]->position.x);
        Tile &new_tile = getTile(m_pieces[i]->position.y, m_pieces[i]->position.x, cols);

        old_tile.setPiece(Tile::INVALID_INDEX);
        new_tile.setPiece(i);
    }

    bool changed_x = cols != m_board_dimensions.x;
    m_board_dimensions.y = rows;
    m_board_dimensions.x = cols;

    generateBoard(m_primary_tile, m_secondary_tile, changed_x);
}

auto Board::getTile(size_t row, size_t col, size_t row_size) -> Tile & {
    if (row_size)
        return m_tiles[row * row_size + col];
    return m_tiles[row * m_board_dimensions.x + col];
}

auto Board::generateBoard(sf::Color primary_color, sf::Color secondary_color, bool changed_x) -> void {
    m_tile_size = sf::Vector2f{m_viewport.x / static_cast<float>(m_board_dimensions.x), m_viewport.y / static_cast<float>(m_board_dimensions.y)};

    std::cout << "=======================================================\n";

    for (size_t row = 0; row < m_board_dimensions.y; row++) {
        for (size_t col = 0; col < m_board_dimensions.x; col++) {
            Tile &curr_tile = getTile(row, col);

            curr_tile.board_pos.x = col;
            curr_tile.board_pos.y = row;
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

            if (curr_tile.getPiece() != curr_tile.INVALID_INDEX) {
                Piece *piece = m_pieces[curr_tile.getPiece()].get();
                sf::Vector2f new_size(m_tile_size.x * 0.9f, m_tile_size.y * 0.9f);
                sf::Sprite &sprite = piece->getSprite();
                if (piece->position.y < 0 || piece->position.y > m_board_dimensions.y || piece->position.x < 0 || piece->position.x > m_board_dimensions.x) {
                    sprite.setScale(0, 0);
                    continue;
                }

                auto size = resizeSprite(sprite, new_size, changed_x);

                sf::Vector2f pos = curr_tile.getPosition();

                Tile &tile = getTile(piece->position.y, piece->position.x);

                std::cout << '(' << piece->position.x << ',' << piece->position.y << ") -> ";

                pos.x += m_tile_size.x * 0.5f - size.x * 0.5f;
                pos.y += m_tile_size.y * 0.5f - size.y * 0.5f;
                sprite.setPosition(pos);
                std::cout << '(' << piece->getSprite().getPosition().x << ',' << piece->getSprite().getPosition().y << ")\n";
            }
            curr_tile.original_color = tile_color;
            curr_tile.setFillColor(tile_color);
        }
    }
}

auto Board::isMouseInBounds(sf::Vector2f mouse) -> bool { return m_borders.getGlobalBounds().contains(mouse); }

auto Board::clearBoard() -> void {
    for (auto &piece : m_pieces) {
        getTile(piece->position.y, piece->position.x).setPiece(Tile::INVALID_INDEX);
        piece.reset(nullptr);
    }
    m_pieces.clear();
}

auto Board::getPiece(size_t index) -> std::unique_ptr<Piece> & { return m_pieces[index]; }

auto Tile::setPiece(size_t index) -> void { this->m_piece_index = index; }

auto Tile::getPiece() -> size_t { return m_piece_index; }