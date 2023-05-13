// clang-format off
#include "pch.hpp"
// clang-format on

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
    m_need_update = true;
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
    return {static_cast<uint32_t>(mouse_position.x / m_tile_size.x), static_cast<uint32_t>(mouse_position.y / m_tile_size.y)};
}

auto Board::executeMove(const Move &move) -> std::unique_ptr<Piece> & {
    m_need_update = true;
    Tile &tile_dest = getTile(move.getMoveDestination());
    Tile &tile_orig = getTile(move.getMoveOrigin());
    std::unique_ptr<Piece> &bystander = tile_dest.piece;
    std::unique_ptr<Piece> &mover = tile_orig.piece;

    mover->position = move.getMoveDestination();

    auto pos = tile_dest.getPosition();
    auto size = mover->getSprite().getTexture()->getSize();
    size.x *= mover->getSprite().getScale().x;
    size.y *= mover->getSprite().getScale().y;
    pos.x += m_tile_size.x * 0.5f - size.x * 0.5f;
    pos.y += m_tile_size.y * 0.5f - size.y * 0.5f;
    mover->getSprite().setPosition(pos);
    mover->position = move.getMoveDestination();

    mover.swap(bystander);
    mover.reset(nullptr);

    return bystander;
}

auto Board::getMoves(Match *match, Piece *piece) -> std::vector<Move> {
    std::vector<Move> moves;
    auto isValid = [this](Move &move) {
        auto inBound = [&](sf::Vector2u vec) { return vec.x < 0 || vec.x > this->m_board_dimensions.x || vec.y < 0 || vec.y > this->m_board_dimensions.y; };
        return inBound(move.getMoveDestination()) || inBound(move.getMoveOrigin());
    };
    for (auto move : piece->getMoves(match)) {
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

    tile.piece.reset(piece);
}

auto Board::removePiece(sf::Vector2u pos) -> void { getTile(pos).piece.reset(nullptr); }

auto Board::highlightMoves(Match *match, Piece *piece) -> void {
    auto &moves = piece->getMoves(match);

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
    return tile.piece.get();
}

auto Board::clearMoves(const Piece *moved_piece) -> void {
    if (!moved_piece)
        return;
    for (auto &coord : moved_piece->getOldMoves()) {
        Tile &tile = getTile(coord.getMoveDestination().y, coord.getMoveDestination().x);
        tile.setFillColor(tile.original_color);
        tile.move.reset(nullptr);
    }
}

auto Board::draw(sf::RenderTarget &target, sf::RenderStates states) const -> void {
    if (!m_need_update)
        return;

    target.draw(m_borders);
    for (size_t row = 0; row < m_board_dimensions.y; row++) {
        for (size_t col = 0; col < m_board_dimensions.x; col++) {
            const Tile &curr_tile = this->getTile(row, col);
            target.draw(curr_tile);
            if (curr_tile.piece)
                target.draw(curr_tile.piece->getSprite());
        }
    }
}

auto Board::handleMouseClick(sf::FloatRect mouse) -> Tile * {
    Tile *tile = getTileFromMousePostion({mouse.left, mouse.top});
    return tile;
}

auto Board::resize(sf::Vector2u board_dimensions) -> void { resize(board_dimensions.x, board_dimensions.y); }
auto Board::resize(uint32_t cols, uint32_t rows) -> void {
    if (cols == 0 || rows == 0 || (rows == m_board_dimensions.y && cols == m_board_dimensions.x))
        return;

    if (rows * cols > m_tiles.capacity())
        m_tiles.reserve(rows * cols);

    for (size_t i = m_tiles.size(); i < m_tiles.capacity(); i++)
        m_tiles.emplace_back(sf::Color::White, nullptr);

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
auto Board::getTile(sf::Vector2u pos, size_t row_size) -> Tile & { return getTile(pos.y, pos.x, row_size); }
auto Board::getTile(size_t row, size_t col, size_t row_size) const -> const Tile & {
    if (row_size)
        return m_tiles[row * row_size + col];
    return m_tiles[row * m_board_dimensions.x + col];
}
auto Board::getTile(sf::Vector2u pos, size_t row_size) const -> const Tile & { return getTile(pos.y, pos.x, row_size); }

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

            if (curr_tile.piece) {
                Piece *piece = curr_tile.piece.get();
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
    for (auto &tile : m_tiles) {
        if (tile.piece)
            tile.piece.reset(nullptr);
    }
}
