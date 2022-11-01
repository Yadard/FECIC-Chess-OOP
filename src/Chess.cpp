#include "Chess.hpp"

Chess::Chess()
    : _render(sf::VideoMode(1028, 960), "Chess FECIC"), _board({0, 100}, {850, 760}), _history({849, 0}, {188, 960}),
      _killzones({{0, 0}, {850, 100}}, {{0, 860}, {850, 100}}) {

    this->_event_handlers[sf::Event::EventType::MouseButtonPressed] = std::bind(&Chess::onMouseButtonPress, this, std::placeholders::_1);
    this->_event_handlers[sf::Event::EventType::Closed] = std::bind(&Chess::onWindowClose, this, std::placeholders::_1);
}

auto Chess::run() -> void {
    while (_render.isOpen()) {

        sf::Event event;
        while (_render.pollEvent(event)) {
            if (this->_event_handlers[event.type])
                this->_event_handlers[event.type](event);
        }

        // clear the window with black color
        _render.clear(sf::Color::Black);

        // draw everything here...
        if (!_drawn) {
            _killzones.starter_team.draw(_render);
            _killzones.last_team.draw(_render);
            _history.draw(_render);
            _board.draw(_render);
            _drawn = true;
        } else {
            _killzones.starter_team.update(_render);
            _killzones.last_team.update(_render);
            _history.update(_render);
            _board.update(_render);
        }

        // end the current frame
        _render.display();
    }
}

auto Chess::win(Team winner) -> void { _render.close(); }

auto Chess::addPiece(std::unique_ptr<Piece> piece) -> void {
    if (piece->getTeam() == Team::STARTER) {
        piece->getSprite().setColor(sf::Color::White);
        ++pieces_amount.starter;
    } else {
        piece->getSprite().setColor(sf::Color::Black);
        ++pieces_amount.last;
    }
    _board.addPiece(piece);
}

auto Chess::onMouseButtonPress(sf::Event &event) -> void {
    if (event.mouseButton.button == sf::Mouse::Button::Left) {
        Tile *tile = _board.getTileFromMousePostion({event.mouseButton.x, event.mouseButton.y});
        if (selected_piece) {
            if (tile->move.get()) {
                std::unique_ptr<Piece> dest = std::move(_board.executeMove(*tile->move));
                if (dest) {
                    if (current_turn == Team::STARTER) {
                        _killzones.last_team.addPiece(dest->getSprite());
                        dest->onDie();
                    } else {
                        _killzones.starter_team.addPiece(dest->getSprite());
                        dest->onDie();
                    }
                    dest.release();
                }

                _history.registryMove(*tile->move);
                _board.clearHighlightMoves(selected_piece);
                selected_piece = nullptr;
                endTurn();
            } else {
                selected_piece = nullptr;
                _board.clearHighlightMoves(selected_piece);
            }
        } else {
            if (tile->piece) {
                if (tile->piece->getTeam() == current_turn) {
                    selected_piece = tile->piece.get();
                    _board.highlightMoves(selected_piece);
                }
            }
        }
    } else if (event.mouseButton.button == sf::Mouse::Button::Right) {
        selected_piece = nullptr;
        _board.clearHighlightMoves(selected_piece);
    }
}

auto Chess::onWindowClose(sf::Event &event) -> void { _render.close(); }

auto Chess::endTurn() -> void { current_turn = static_cast<Team>((static_cast<int>(current_turn) + 1) % 2); }