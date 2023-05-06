#ifndef PRESET_H
#define PRESET_H

#include <SFML/Graphics.hpp>
#include <Scenes/Chess/Pieces/Piece.hpp>
#include <exception>
#include <filesystem>
#include <fstream>
#include <string>

class Preset {
  public:
    class ParsingError : public std::exception {
      public:
        ParsingError(const std::string &errorMsg) : errorMsg_(errorMsg) {}
        const char *what() const noexcept override { return errorMsg_.c_str(); }

      private:
        std::string errorMsg_;
    };

    struct ParsingInfo {
        std::string_view file_name;
        std::string line_raw;
        size_t line_num;
    };

    struct PieceInfo {
        std::string name;
        sf::Vector2u board_pos;
        Team team;
    };

    auto loadFromFile(const char *path) -> bool;
    auto loadFromFile(const std::filesystem::path &path) -> bool;

    auto getBoardSize() const -> sf::Vector2u;
    auto getName() const -> const std::string &;
    auto getPiecesInfo() const -> const std::vector<PieceInfo> &;

  private:
    auto parsePiece(ParsingInfo parsing_info) -> void;

    auto stripIdentifiers(std::string_view id) -> std::string;
    auto isValidPieceIdentifier(std::string_view id) -> bool;

    auto isOpeningToken(const char c) -> bool;
    auto isClosingToken(const char c) -> bool;

    sf::Vector2u m_board_size;
    std::string m_name;

    std::vector<PieceInfo> m_pieces;
};

#endif // PRESET_H