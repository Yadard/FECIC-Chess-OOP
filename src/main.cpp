#include "Chess.hpp"
#include "Pieces/Bishop.hpp"
#include "Pieces/King.hpp"
#include "Pieces/Knight.hpp"
#include "Pieces/Pawn.hpp"
#include "Pieces/Queen.hpp"
#include "Pieces/Rook.hpp"
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>

void setSpriteSize(sf::Sprite &sprite, sf::Vector2f size) {
    float factor;
    if (sprite.getTextureRect().width > sprite.getTextureRect().height) {
        factor = size.x / sprite.getTextureRect().width;
    } else {
        factor = size.y / sprite.getTextureRect().height;
    }
    sprite.setScale(factor, factor);
}

void setup_tradicional_chess(Chess &chess, sf::Texture *black, sf::Texture *white) {
    std::string base("./assets/sprites/");
    const char *names[] = {"pawn", "bishop", "knight", "king", "queen", "rook"};

    bool sucess = true;
    for (size_t i = 0; i < 6; i++) {
        std::string w(base + "white/" + names[i] + ".png");
        std::string b(base + "black/" + names[i] + ".png");
        if (!white[i].loadFromFile(w.c_str()) || !black[i].loadFromFile(b.c_str())) {
            sucess = false;
        }
    }

    sf::Sprite t_sprite;
    for (size_t i = 0; i < 8; i++) {
        t_sprite.setTexture(white[0]);
        setSpriteSize(t_sprite, {80.0, 80.0});
        chess.addPiece(std::make_unique<Pawn>(Team::STARTER, Move::BoardPos(i, 1), t_sprite));
        t_sprite.setTexture(black[0]);
        setSpriteSize(t_sprite, {80.0, 80.0});
        chess.addPiece(std::make_unique<Pawn>(Team::LATTER, Move::BoardPos(i, 6), t_sprite));
    }

    for (size_t i = 0; i < 8; i++) {
        if (i == 0 || i == 7) {
            t_sprite.setTexture(white[5]);
            setSpriteSize(t_sprite, {80.0, 80.0});
            chess.addPiece(std::make_unique<Rook>(Team::STARTER, Move::BoardPos(i, 0), t_sprite));
            t_sprite.setTexture(black[5]);
            setSpriteSize(t_sprite, {80.0, 80.0});
            chess.addPiece(std::make_unique<Rook>(Team::LATTER, Move::BoardPos(i, 7), t_sprite));
        } else if (i == 1 || i == 6) {
            t_sprite.setTexture(white[2]);
            setSpriteSize(t_sprite, {80.0, 80.0});
            chess.addPiece(std::make_unique<Knight>(Team::STARTER, Move::BoardPos(i, 0), t_sprite));
            t_sprite.setTexture(black[2]);
            setSpriteSize(t_sprite, {80.0, 80.0});
            chess.addPiece(std::make_unique<Knight>(Team::LATTER, Move::BoardPos(i, 7), t_sprite));
        } else if (i == 2 || i == 5) {
            t_sprite.setTexture(white[1]);
            setSpriteSize(t_sprite, {80.0, 80.0});
            chess.addPiece(std::make_unique<Bishop>(Team::STARTER, Move::BoardPos(i, 0), t_sprite));
            t_sprite.setTexture(black[1]);
            setSpriteSize(t_sprite, {80.0, 80.0});
            chess.addPiece(std::make_unique<Bishop>(Team::LATTER, Move::BoardPos(i, 7), t_sprite));
        } else if (i == 3) {
            t_sprite.setTexture(white[4]);
            setSpriteSize(t_sprite, {80.0, 80.0});
            chess.addPiece(std::make_unique<Queen>(Team::STARTER, Move::BoardPos(i, 0), t_sprite));
            t_sprite.setTexture(black[4]);
            setSpriteSize(t_sprite, {80.0, 80.0});
            chess.addPiece(std::make_unique<Queen>(Team::LATTER, Move::BoardPos(i, 7), t_sprite));
        } else {
            t_sprite.setTexture(white[3]);
            setSpriteSize(t_sprite, {80.0, 80.0});
            chess.addPiece(std::make_unique<King>(Team::STARTER, Move::BoardPos(i, 0), t_sprite, std::bind(&Chess::win, &chess, Team::LATTER)));
            t_sprite.setTexture(black[3]);
            setSpriteSize(t_sprite, {80.0, 80.0});
            chess.addPiece(std::make_unique<King>(Team::LATTER, Move::BoardPos(i, 7), t_sprite, std::bind(&Chess::win, &chess, Team::STARTER)));
        }
    }
}

int main(int argc, char const *argv[]) {
    Chess chess;
    sf::Texture white[6], black[6];
    setup_tradicional_chess(chess, black, white);
    chess.run();

    return 0;
}