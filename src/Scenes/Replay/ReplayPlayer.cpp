#include "Scenes/Replay/ReplayPlayer.hpp"

Scene::ReplayPlayer::ReplayPlayer(sf::RenderWindow &render, std::function<void()> t_quit, std::function<void(IScene *)> t_change_scene, const Replay &replay)
    : IScene(render, t_quit, t_change_scene), m_board({5, 95}, {840, 750}, replay.getBoardSize()), m_history({849, 0}, {188, 960}),
      m_killzones({{0, 0}, {850, 100}}, {{0, 860}, {850, 100}}), m_replay(replay) {
    loadBoard(replay);
    m_time = sf::Time::Zero;
    m_current_move = m_replay.getMoves().cbegin();
    m_viewport = sf::Vector2f(render.getSize().x, render.getSize().y);
    m_move_sfx.setBuffer(AssetManager::GetInstance().getSFX("MainMenu.PieceMove"));
}

auto Scene::ReplayPlayer::update(sf::RenderWindow &render) -> void {

    this->draw(render);
    m_time += m_clock.restart();
    if (m_time >= sf::seconds(1)) {
        executeMove(*m_current_move);
        std::advance(m_current_move, 1);
        if (m_current_move == m_replay.getMoves().cend()) {
            win(Team::UNKNOWN);
        }
        m_time = sf::Time::Zero;
    }
}
auto Scene::ReplayPlayer::draw(sf::RenderWindow &render) -> void {
    m_killzones.starter_team.draw(render);
    m_killzones.last_team.draw(render);
    render.draw(m_history);
    render.draw(m_board);
}

auto Scene::ReplayPlayer::win(Team team) -> void { m_change_scene(new Scene::MainMenu(m_render, m_quit, m_change_scene)); }

auto Scene::ReplayPlayer::createPiece(std::string name, Team team, sf::Vector2u pos) -> Piece * {
    return AssetManager::GetInstance().piece_factory.createPiece(name, team, pos);
}

auto Scene::ReplayPlayer::addPiece(Piece *piece, sf::Vector2u pos) -> void {
    if (piece->getTeam() == Team::BLACK)
        m_pieces_amount.black++;
    else
        m_pieces_amount.white++;
    m_board.addPiece(piece, pos);
}
auto Scene::ReplayPlayer::hasPiece(sf::Vector2u pos) const -> Piece * { return m_board.getTile(pos).piece.get(); }
auto Scene::ReplayPlayer::getBoardSize() const -> sf::Vector2u { return m_board.getBoardDimensions(); }

auto Scene::ReplayPlayer::handle_input(std::function<void(Scene::IScene *)> change_scene, std::function<void()> quit, sf::Event event) -> void {

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

auto Scene::ReplayPlayer::handleMousePress(sf::Event event) -> void {}

auto Scene::ReplayPlayer::executeMove(const Move &move) -> Piece * {
    Tile &tile_dest = m_board.getTile(move.getMoveDestination().y, move.getMoveDestination().x);
    Tile &tile_orig = m_board.getTile(move.getMoveOrigin().y, move.getMoveOrigin().x);

    if (move.getMoveDestination() == move.getMoveOrigin()) {
        tile_orig.piece->doSpecialMove(this);
        return tile_orig.piece.get();
    }

    m_history.registryMove(move, tile_orig.piece->getSprite());
    const Piece *bystander = tile_dest.piece.get();
    m_selected_piece = tile_orig.piece.get();

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
    m_move_sfx.play();
    return mover.get();
}

auto Scene::ReplayPlayer::loadBoard(const Replay &replay) -> void {
    for (auto &piece : replay.getPiecesInfo()) {
        if (piece.team == Team::WHITE)
            m_pieces_amount.white++;
        else
            m_pieces_amount.black++;
        Piece *p = AssetManager::GetInstance().piece_factory.createPiece(piece.name, piece.team, piece.board_pos);
        m_board.addPiece(p, piece.board_pos);
    }
}

auto Scene::ReplayPlayer::highlightMoves(Piece *piece) -> void { m_board.highlightMoves(this, piece); }
auto Scene::ReplayPlayer::clearMoves(const Piece *piece, sf::Vector2u pos) -> void {
    Tile &tile = m_board.getTile(pos.y, pos.x);
    tile.setOutlineThickness(0);
    tile.setFillColor(tile.original_color);
    m_board.clearMoves(piece);
    m_selected_piece = nullptr;
}

auto Scene::ReplayPlayer::reachedEnemyField(const Move &move) -> bool {
    if (m_selected_piece->getTeam() == Team::WHITE)
        return move.getMoveDestination().y == m_board.getBoardDimensions().y - 1;
    else
        return move.getMoveDestination().y == 0;
}

auto Scene::ReplayPlayer::handleKeyboardPress(sf::Event event) -> void {}

auto Scene::ReplayPlayer::endTurn() -> void {
    if (m_pieces_amount.black == 0)
        win(Team::WHITE);
    else if (m_pieces_amount.white == 0)
        win(Team::BLACK);
    m_current_turn = static_cast<Team>((static_cast<int>(m_current_turn) + 1) % 2);
}