// clang-format off
#include "pch.hpp"
// clang-format on

#include "Scenes/Chess/Chess.hpp"
#include "AssetManager.hpp"

Scene::Chess::Chess(sf::RenderWindow &render, std::function<void()> t_quit, std::function<void(IScene *)> t_change_scene, Preset &preset, bool new_preset)
    : IScene(render, t_quit, t_change_scene), m_board({5, 95}, {840, 750}, preset.getBoardSize()), m_history({849, 0}, {188, 960}),
      m_killzones({{0, 0}, {850, 100}}, {{0, 860}, {850, 100}}), m_preset(preset), m_new_preset(new_preset), m_victory(nullptr) {
    loadPresetOnBoard(preset);
    m_viewport = sf::Vector2f(render.getSize().x, render.getSize().y);
}

auto Scene::Chess::update(sf::RenderWindow &render) -> void {
    m_killzones.starter_team.update(render);
    m_killzones.last_team.update(render);
    m_history.update(render);
    m_board.update(render);

    if (m_victory)
        render.draw(*m_victory);
}
auto Scene::Chess::draw(sf::RenderWindow &render) -> void {
    m_killzones.starter_team.draw(render);
    m_killzones.last_team.draw(render);
    m_history.draw(render);
    m_board.draw(render);

    if (m_victory)
        render.draw(*m_victory);
}

auto Scene::Chess::win(Team team) -> void {
    m_victory.reset(new Victory(m_render, m_quit, m_change_scene, team, m_preset, m_new_preset));
    ;
}

auto Scene::Chess::createPiece(std::string name, Team team, sf::Vector2u pos) -> Piece * {
    return AssetManager::GetInstance().piece_factory.createPiece(name, team, pos);
}

auto Scene::Chess::handle_input(std::function<void(Scene::IScene *)> change_scene, std::function<void()> quit, sf::Event event) -> void {
    if (m_victory)
        m_victory->handleInput(event);
    switch (event.type) {
    case sf::Event::MouseButtonPressed:
        handleMousePress(event);
        break;

    case sf::Event::MouseButtonReleased:
        /* code */
        break;

    case sf::Event::KeyPressed:
        handleKeyboardPress(event);
        break;

    case sf::Event::KeyReleased:
        /* code */
        break;

    default:
        break;
    }
}

auto Scene::Chess::handleMousePress(sf::Event event) -> void {
    sf::Vector2f mouse(event.mouseButton.x, event.mouseButton.y);
    if (event.mouseButton.button == sf::Mouse::Left) {
        if (!m_board.isMouseInBounds(mouse))
            return;

        Tile &selected_tile = *m_board.getTileFromMousePostion(mouse);

        if (m_selected_piece) {
            sf::Vector2u pos = m_selected_piece->position;
            Piece *piece = selected_tile.move->getPiece();
            if (selected_tile.move) {
                piece = executeMove(*selected_tile.move);
            }
            clearMoves(piece, pos);
            endTurn();
        } else {
            Piece *piece = nullptr;
            if (selected_tile.getPiece() != Tile::INVALID_INDEX)
                piece = m_board.getPiece(selected_tile.getPiece()).get();

            if (piece && piece->getTeam() == m_current_turn) {
                m_selected_piece = piece;
                highlightMoves(selected_tile.getPiece());
            }
        }

    } else if (event.mouseButton.button == sf::Mouse::Right) {
        if (m_selected_piece)
            clearMoves(m_selected_piece, m_selected_piece->position);
    }
}

auto Scene::Chess::executeMove(Move &move) -> Piece * {
    m_history.registryMove(move);
    Tile &tile_dest = m_board.getTile(move.getMoveDestination().y, move.getMoveDestination().x);
    Tile &tile_orig = m_board.getTile(move.getMoveOrigin().y, move.getMoveOrigin().x);

    const Piece *bystander = nullptr;
    if (tile_dest.getPiece() != Tile::INVALID_INDEX)
        bystander = m_board.getPiece(tile_dest.getPiece()).get();

    if (bystander) {
        if (m_current_turn == Team::WHITE) {
            m_killzones.last_team.addPiece(bystander->getSprite());
            m_pieces_amount.black--;
        } else {
            m_pieces_amount.white--;
            m_killzones.starter_team.addPiece(bystander->getSprite());
        }

        if (bystander->onDie)
            bystander->onDie(this);
    }
    std::unique_ptr<Piece> &mover = m_board.executeMove(move);
    if (mover->onMove)
        mover->onMove(this, move.getMoveDestination());
    if (reachedEnemyField(move) && mover->onReachEnd)
        mover->onReachEnd(this, mover);
    return mover.get();
}

auto Scene::Chess::loadPresetOnBoard(Preset &preset) -> void {
    for (auto &piece : preset.getPiecesInfo()) {
        if (piece.team == Team::WHITE)
            m_pieces_amount.white++;
        else
            m_pieces_amount.black++;
        Piece *p = AssetManager::GetInstance().piece_factory.createPiece(piece.name, piece.team, piece.board_pos);
        m_board.addPiece(p, piece.board_pos);
    }
}

auto Scene::Chess::highlightMoves(size_t piece_index) -> void { m_board.highlightMoves(piece_index); }
auto Scene::Chess::clearMoves(const Piece *piece, sf::Vector2u pos) -> void {
    Tile &tile = m_board.getTile(pos.y, pos.x);
    tile.setOutlineThickness(0);
    tile.setFillColor(tile.original_color);
    m_board.clearMoves(piece);
    m_selected_piece = nullptr;
}

auto Scene::Chess::reachedEnemyField(Move &move) -> bool {
    if (m_selected_piece->getTeam() == Team::WHITE)
        return move.getMoveDestination().y == m_board.getBoardDimensions().y - 1;
    else
        return move.getMoveDestination().y == 0;
}

auto Scene::Chess::handleKeyboardPress(sf::Event event) -> void {}

auto Scene::Chess::endTurn() -> void {
    if (m_pieces_amount.black == 0)
        win(Team::WHITE);
    else if (m_pieces_amount.white == 0)
        win(Team::BLACK);
    m_current_turn = static_cast<Team>((static_cast<int>(m_current_turn) + 1) % 2);
}

/*
auto Scene::Chess::win(Team winner) -> void { _render.close(); }

auto Scene::Chess::addPiece(std::unique_ptr<Piece> piece) -> void {
    if (piece->getTeam() == Team::WHITE) {
        piece->getSprite().setColor(sf::Color::White);
        ++m_pieces_amount.white;
    } else {
        piece->getSprite().setColor(sf::Color::Black);
        ++m_pieces_amount.black;
    }
    m_board.addPiece(piece);
}

auto Scene::Chess::onMouseButtonPress(sf::Event &event) -> void {
    if (event.mouseButton.button == sf::Mouse::Button::Left) {
        Tile *tile = m_board.getTileFromMousePostion({(float)event.mouseButton.x, (float)event.mouseButton.y});
        if (!tile)
            return;
        if (m_selected_piece && *m_selected_piece) {
            if (tile->move.get()) {
                bool reached_end = false;
                if ((*m_selected_piece)->getTeam() == Team::WHITE) {
                    reached_end = tile->move->getMoveDestination().y == 7;
                } else {
                    reached_end = tile->move->getMoveDestination().y == 0;
                }
                std::unique_ptr<Piece> &dest = m_board.executeMove(*tile->move);
                Move move(dest.get(), tile->move->getMoveDestination(), tile->move->getMoveOrigin());
                m_history.registryMove(move);
                m_board.clearHighlightMoves(dest.get());

                if (dest->onMove)
                    dest->onMove(tile->move->getMoveDestination());
                if (reached_end && dest->onReachEnd)
                    dest->onReachEnd(dest, this->sprites);
                if (m_selected_piece && *m_selected_piece) {
                    if (m_current_turn == Team::WHITE) {
                        m_killzones.last_team.addPiece((*m_selected_piece)->getSprite());
                    } else {
                        m_killzones.starter_team.addPiece((*m_selected_piece)->getSprite());
                    }
                    if ((*m_selected_piece)->onDie)
                        (*m_selected_piece)->onDie();
                    m_selected_piece->release();
                }

                m_selected_piece = nullptr;
                tile->move.reset();
                endTurn();
            } else {
                m_board.clearHighlightMoves(m_selected_piece->get());
                m_selected_piece = nullptr;
            }
        } else {
            if (tile->piece) {
                if (tile->piece->getTeam() == m_current_turn) {
                    m_selected_piece = &tile->piece;
                    m_board.highlightMoves(m_selected_piece->get());
                }
            }
        }
    } else if (event.mouseButton.button == sf::Mouse::Button::Right) {
        if (m_selected_piece && *m_selected_piece) {
            m_board.clearHighlightMoves(m_selected_piece->get());
            m_selected_piece = nullptr;
        }
    }
}

auto Scene::Chess::onWindowClose(sf::Event &event) -> void { _render.close(); }

*/