#include "Move.hpp"

Move::Move(Piece *t_piece, BoardPos t_destination, BoardPos t_origin) : _piece(t_piece), _destination(t_destination), _origin(t_origin) {}
auto Move::getPiece() -> Piece * { return _piece; }
auto Move::getMoveDestination() -> BoardPos { return _destination; }
auto Move::getMoveOrigin() -> BoardPos { return _origin; }