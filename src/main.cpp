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
    std::string base("./../assets/sprites/");
    const char *names[] = {"pawn", "bishop", "knight", "king", "queen", "rook"};

    bool success = true;
    for (size_t i = 0; i < 6; i++) {
        std::string w(base + "white/" + names[i] + ".png");
        std::string b(base + "black/" + names[i] + ".png");
        if (!white[i].loadFromFile(w.c_str()) || !black[i].loadFromFile(b.c_str())) {
            success = false;
        }
    }

    sf::Sprite t_sprite;
    for (size_t i = 0; i < 6; i++) {
        t_sprite.setTexture(white[i]);
        setSpriteSize(t_sprite, {80.0, 80.0});
        std::string key = "Starter_";
        key += names[i];
        chess.registerSprite(key, t_sprite);
        key = "Latter_";
        key += names[i];
        t_sprite.setTexture(black[i]);
        setSpriteSize(t_sprite, {80.0, 80.0});
        chess.registerSprite(key, t_sprite);
    }

    for (size_t i = 0; i < 8; i++) {
        chess.addPiece(std::make_unique<Pawn>(Team::STARTER, Move::BoardPos(i, 1), chess.getSprite("Starter_pawn")));
        chess.addPiece(std::make_unique<Pawn>(Team::LATTER, Move::BoardPos(i, 6), chess.getSprite("Latter_pawn")));
    }

    for (size_t i = 0; i < 8; i++) {
        switch (i) {
        case 7:
        case 0:
            chess.addPiece(std::make_unique<Rook>(Team::STARTER, Move::BoardPos(i, 0), chess.getSprite("Starter_rook")));
            chess.addPiece(std::make_unique<Rook>(Team::LATTER, Move::BoardPos(i, 7), chess.getSprite("Latter_rook")));
            break;

        case 1:
        case 6:
            chess.addPiece(std::make_unique<Knight>(Team::STARTER, Move::BoardPos(i, 0), chess.getSprite("Starter_knight")));
            chess.addPiece(std::make_unique<Knight>(Team::LATTER, Move::BoardPos(i, 7), chess.getSprite("Latter_knight")));
            break;

        case 2:
        case 5:
            chess.addPiece(std::make_unique<Bishop>(Team::STARTER, Move::BoardPos(i, 0), chess.getSprite("Starter_bishop")));
            chess.addPiece(std::make_unique<Bishop>(Team::LATTER, Move::BoardPos(i, 7), chess.getSprite("Latter_bishop")));
            break;

        case 3:
            chess.addPiece(std::make_unique<Queen>(Team::STARTER, Move::BoardPos(i, 0), chess.getSprite("Starter_queen")));
            chess.addPiece(std::make_unique<Queen>(Team::LATTER, Move::BoardPos(i, 7), chess.getSprite("Latter_queen")));
            break;

        case 4:
            chess.addPiece(
                std::make_unique<King>(Team::STARTER, Move::BoardPos(i, 0), chess.getSprite("Starter_king"), std::bind(&Chess::win, &chess, Team::LATTER)));
            chess.addPiece(
                std::make_unique<King>(Team::LATTER, Move::BoardPos(i, 7), chess.getSprite("Latter_king"), std::bind(&Chess::win, &chess, Team::STARTER)));
            break;

        default:
            break;
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