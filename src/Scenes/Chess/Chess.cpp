// clang-format off
#include "pch.hpp"
// clang-format on

#include "Scenes/Chess/Chess.hpp"
#include "AssetManager.hpp"

Scene::Chess::Chess(sf::RenderWindow &render, std::function<void()> t_quit, std::function<void(IScene *)> t_change_scene, Preset &preset, bool new_preset)
    : IScene(render, t_quit, t_change_scene), m_board({5, 95}, {840, 750}, preset.getBoardSize()), m_history({849, 0}, {188, 960}),
      m_killzones({{0, 0}, {850, 100}}, {{0, 860}, {850, 100}}), m_preset(preset), m_new_preset(new_preset), m_match_result(nullptr) {
    loadPresetOnBoard(preset);
    m_viewport = sf::Vector2f(render.getSize().x, render.getSize().y);
    m_move_sfx.setBuffer(AssetManager::GetInstance().getSFX("MainMenu.PieceMove"));

    m_replay.copyPreset(preset);
}

auto Scene::Chess::update(sf::RenderWindow &render) -> void {
    if (this->m_match_result)
        this->m_match_result->update();
    this->draw(render);
}
auto Scene::Chess::draw(sf::RenderWindow &render) -> void {
    m_killzones.starter_team.draw(render);
    m_killzones.last_team.draw(render);
    render.draw(m_history);
    render.draw(m_board);

    if (m_match_result)
        render.draw(*m_match_result);
}

auto Scene::Chess::win(Team team) -> void {
    auto mainmenu = [this]() { this->m_change_scene(new Scene::MainMenu(m_render, m_quit, m_change_scene)); };
    auto play_again = [this]() { this->m_change_scene(new Scene::Chess(m_render, m_quit, m_change_scene, m_preset, m_new_preset)); };
    sf::Vector2f viewport(m_render.getSize().x, m_render.getSize().y);
    m_match_result.reset(new Victory(viewport, mainmenu, play_again, m_preset, m_new_preset, m_replay, team));
}

auto Scene::Chess::createPiece(std::string name, Team team, sf::Vector2u pos) -> Piece * {
    return AssetManager::GetInstance().piece_factory.createPiece(name, team, pos);
}

auto Scene::Chess::addPiece(Piece *piece, sf::Vector2u pos) -> void {
    if (piece->getTeam() == Team::BLACK)
        m_pieces_amount.black++;
    else
        m_pieces_amount.white++;
    m_board.addPiece(piece, pos);
}
auto Scene::Chess::hasPiece(sf::Vector2u pos) const -> Piece * { return m_board.getTile(pos).piece.get(); }
auto Scene::Chess::getBoardSize() const -> sf::Vector2u { return m_board.getBoardDimensions(); }

auto Scene::Chess::handle_input(std::function<void(Scene::IScene *)> change_scene, std::function<void()> quit, sf::Event event) -> void {
    if (m_match_result) {
        m_match_result->handleInput(event);
        return;
    }

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
            if (!selected_tile.move)
                return;

            sf::Vector2u pos = m_selected_piece->position;
            Piece *piece = nullptr;
            if (selected_tile.move) {
                piece = executeMove(*selected_tile.move);
                m_replay.addMove(*selected_tile.move);
                m_move_sfx.play();
                endTurn();
            }

            clearMoves(piece, pos);
        } else {
            Piece *piece = nullptr;
            if (selected_tile.piece)
                piece = selected_tile.piece.get();

            if (piece && piece->getTeam() == m_current_turn) {
                m_selected_piece = piece;
                highlightMoves(piece);
            }
        }

    } else if (event.mouseButton.button == sf::Mouse::Right) {
        if (m_selected_piece) {
            clearMoves(m_selected_piece, m_selected_piece->position);
            return;
        }

        if (!m_board.isMouseInBounds(mouse))
            return;

        Tile &selected_tile = *m_board.getTileFromMousePostion(mouse);

        if (selected_tile.piece && selected_tile.piece->getTeam() == m_current_turn) {
            selected_tile.piece->doSpecialMove(this);
            m_replay.addMove(Move(selected_tile.board_pos, selected_tile.board_pos));
        }
    }
}

auto Scene::Chess::executeMove(Move &move) -> Piece * {
    Tile &tile_dest = m_board.getTile(move.getMoveDestination().y, move.getMoveDestination().x);
    Tile &tile_orig = m_board.getTile(move.getMoveOrigin().y, move.getMoveOrigin().x);

    m_history.registryMove(move, tile_orig.piece->getSprite());
    const Piece *bystander = tile_dest.piece.get();

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

auto Scene::Chess::highlightMoves(Piece *piece) -> void { m_board.highlightMoves(this, piece); }
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