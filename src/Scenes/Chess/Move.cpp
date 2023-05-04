#include "Scenes/Chess/Move.hpp"

Move::Move(Piece *t_piece, BoardPos t_destination, BoardPos t_origin) : _piece(t_piece), _destination(t_destination), _origin(t_origin) {}
auto Move::getPiece() const -> Piece * { return _piece; }
auto Move::getMoveDestination() const -> BoardPos { return _destination; }
auto Move::getMoveOrigin() const -> BoardPos { return _origin; }